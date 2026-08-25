/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <windows.h>
#include <winhttp.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proxy_ra.h"
#include "rc_client.h"
#include "rc_consoles.h"

#pragma comment(lib, "winhttp.lib")

#define RA_PROXY_VERSION "0.1.0"
#define RA_USER_AGENT_W L"1964GEPD-RA-Proxy/v0.1.0 (Windows) rcheevos/v12.4.0"

typedef struct ra_http_request {
  char* url;
  char* post_data;
  rc_client_server_callback_t callback;
  void* callback_data;
} ra_http_request;

typedef struct ra_notice {
  char title[96];
  char text[512];
} ra_notice;

static rc_client_t* g_ra_client;
static char g_ra_ini[MAX_PATH];
static volatile LONG g_ra_http_count;
static HANDLE g_ra_http_idle;
static int g_ra_shutting_down;
static const unsigned char** g_ra_lut;
static const unsigned char* g_ra_rom;
static size_t g_ra_rom_size;
static volatile LONG g_ra_memory_reads;
static unsigned long g_ra_frames;
static int g_ra_evaluation_logged;

static void ra_log_line(const char* message) {
  FILE* file = fopen("retroachievements.log", "a");
  if (file) { fprintf(file, "%s\n", message); fclose(file); }
}

static unsigned __stdcall ra_notice_thread(void* userdata) {
  ra_notice* notice = (ra_notice*)userdata;
  MessageBoxA(NULL, notice->text, notice->title, MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
  free(notice);
  return 0;
}

static void ra_message(const char* title, const char* text) {
  ra_notice* notice = (ra_notice*)calloc(1, sizeof(*notice));
  uintptr_t thread;
  if (!notice) return;
  _snprintf(notice->title, sizeof(notice->title), "%s", title ? title : "RetroAchievements");
  _snprintf(notice->text, sizeof(notice->text), "%s", text ? text : "");
  thread = _beginthreadex(NULL, 0, ra_notice_thread, notice, 0, NULL);
  if (thread) CloseHandle((HANDLE)thread); else free(notice);
}

static char* ra_strdup(const char* value) {
  size_t size;
  char* copy;
  if (!value) return NULL;
  size = strlen(value) + 1;
  copy = (char*)malloc(size);
  if (copy) memcpy(copy, value, size);
  return copy;
}

static unsigned __stdcall ra_http_thread(void* userdata) {
  ra_http_request* request = (ra_http_request*)userdata;
  rc_api_server_response_t response;
  URL_COMPONENTS parts;
  HINTERNET session = NULL, connection = NULL, handle = NULL;
  wchar_t wide_url[4096], host[256], path[3072], extra[1024];
  DWORD status = 0, status_size = sizeof(status), available = 0, read = 0;
  char* body = NULL;
  size_t body_size = 0;

  memset(&response, 0, sizeof(response));
  memset(&parts, 0, sizeof(parts));
  parts.dwStructSize = sizeof(parts);
  parts.lpszHostName = host; parts.dwHostNameLength = 256;
  parts.lpszUrlPath = path; parts.dwUrlPathLength = 2048;
  parts.lpszExtraInfo = extra; parts.dwExtraInfoLength = 1024;
  MultiByteToWideChar(CP_UTF8, 0, request->url, -1, wide_url, 4096);
  if (!WinHttpCrackUrl(wide_url, 0, 0, &parts)) goto failed;
  if (parts.dwExtraInfoLength && parts.dwUrlPathLength + parts.dwExtraInfoLength < 3071) {
    memcpy(path + parts.dwUrlPathLength, extra, parts.dwExtraInfoLength * sizeof(wchar_t));
    path[parts.dwUrlPathLength + parts.dwExtraInfoLength] = L'\0';
  }

  session = WinHttpOpen(RA_USER_AGENT_W, WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
                        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
  if (!session) goto failed;
  {
    connection = WinHttpConnect(session, host, parts.nPort, 0);
    if (!connection) goto failed;
    handle = WinHttpOpenRequest(connection, request->post_data ? L"POST" : L"GET", path,
      NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
      parts.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
  }
  if (!handle) goto failed;
  WinHttpAddRequestHeaders(handle,
      L"User-Agent: " RA_USER_AGENT_W L"\r\n", -1, WINHTTP_ADDREQ_FLAG_REPLACE);
  if (request->post_data)
    WinHttpAddRequestHeaders(handle, L"Content-Type: application/x-www-form-urlencoded\r\n", -1, WINHTTP_ADDREQ_FLAG_ADD);
  if (!WinHttpSendRequest(handle, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
      request->post_data, request->post_data ? (DWORD)strlen(request->post_data) : 0,
      request->post_data ? (DWORD)strlen(request->post_data) : 0, 0) || !WinHttpReceiveResponse(handle, NULL)) goto failed;
  WinHttpQueryHeaders(handle, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                      NULL, &status, &status_size, NULL);
  for (;;) {
    if (!WinHttpQueryDataAvailable(handle, &available) || !available) break;
    body = (char*)realloc(body, body_size + available + 1);
    if (!body || !WinHttpReadData(handle, body + body_size, available, &read)) goto failed;
    body_size += read;
  }
  if (!body) body = ra_strdup("");
  body[body_size] = '\0';
  response.body = body; response.body_length = body_size; response.http_status_code = (int)status;
  request->callback(&response, request->callback_data);
  goto done;
failed:
  response.body = "Network request failed";
  response.body_length = strlen(response.body);
  response.http_status_code = RC_API_SERVER_RESPONSE_RETRYABLE_CLIENT_ERROR;
  request->callback(&response, request->callback_data);
done:
  if (handle) WinHttpCloseHandle(handle);
  if (connection) WinHttpCloseHandle(connection);
  if (session) WinHttpCloseHandle(session);
  free(body); free(request->url); free(request->post_data); free(request);
  if (InterlockedDecrement(&g_ra_http_count) == 0) SetEvent(g_ra_http_idle);
  return 0;
}

static void ra_server_call(const rc_api_request_t* request, rc_client_server_callback_t callback,
                           void* callback_data, rc_client_t* client) {
  ra_http_request* copy = (ra_http_request*)calloc(1, sizeof(*copy));
  uintptr_t thread;
  (void)client;
  if (!copy) return;
  copy->url = ra_strdup(request->url);
  copy->post_data = ra_strdup(request->post_data);
  copy->callback = callback; copy->callback_data = callback_data;
  ResetEvent(g_ra_http_idle); InterlockedIncrement(&g_ra_http_count);
  thread = _beginthreadex(NULL, 0, ra_http_thread, copy, 0, NULL);
  if (thread) CloseHandle((HANDLE)thread);
  else { InterlockedDecrement(&g_ra_http_count); SetEvent(g_ra_http_idle); free(copy->url); free(copy->post_data); free(copy); }
}

static unsigned int ra_read_memory(unsigned int address, unsigned char* buffer,
                                   unsigned int num_bytes, rc_client_t* client) {
  unsigned int i;
  (void)client;
  if (!g_ra_lut || address > 0x800000 || num_bytes > 0x800000 - address) return 0;
  /* The GEPD HookRDRAM extension exposes 1964's 4KB virtual-memory page table. */
  for (i = 0; i < num_bytes; ++i) {
    unsigned int virtual_address = 0x80000000U + address + i;
    const unsigned char* page = g_ra_lut[virtual_address >> 12];
    if (!page) return i;
    /* 1964 stores RDRAM in the same 32-bit word-swapped byte layout exposed by
       the established N64 RetroAchievements cores. rcheevos definitions are
       authored against that raw layout, so applying an extra XOR-3 here makes
       every byte-sized condition observe the wrong byte. */
    buffer[i] = page[virtual_address & 0xFFF];
  }
  InterlockedIncrement(&g_ra_memory_reads);
  return num_bytes;
}

static void ra_log(const char* message, const rc_client_t* client) {
  FILE* file;
  (void)client;
  file = fopen("retroachievements.log", "a");
  if (file) { fprintf(file, "%s\n", message); fclose(file); }
}

static void ra_event(const rc_client_event_t* event, rc_client_t* client) {
  char message[512];
  (void)client;
  switch (event->type) {
    case RC_CLIENT_EVENT_ACHIEVEMENT_TRIGGERED:
      _snprintf(message, sizeof(message), "%s\n\n%s", event->achievement->title, event->achievement->description);
      {
        char log_message[640];
        _snprintf(log_message, sizeof(log_message), "Achievement triggered: %u \"%s\"",
                  event->achievement->id, event->achievement->title);
        ra_log_line(log_message);
      }
      ra_message("Achievement Unlocked", message); break;
    case RC_CLIENT_EVENT_GAME_COMPLETED: ra_message("RetroAchievements", "Mastery complete!"); break;
    case RC_CLIENT_EVENT_LEADERBOARD_SUBMITTED:
      _snprintf(message, sizeof(message), "%s: %s", event->leaderboard->title, event->leaderboard->tracker_value);
      ra_message("Leaderboard Submitted", message); break;
    default: break;
  }
}

static void ra_load_callback(int result, const char* error, rc_client_t* client, void* userdata) {
  char message[512];
  const rc_client_game_t* game;
  (void)userdata;
  if (result != RC_OK) { _snprintf(message, sizeof(message), "Game identification failed: %s", error ? error : rc_error_str(result)); ra_message("RetroAchievements", message); return; }
  game = rc_client_get_game_info(client);
  g_ra_frames = 0;
  g_ra_evaluation_logged = 0;
  _snprintf(message, sizeof(message), "Achievements active for %s (Casual mode)", game->title);
  ra_message("RetroAchievements", message);
}

static void ra_login_callback(int result, const char* error, rc_client_t* client, void* userdata) {
  char message[512];
  const rc_client_user_t* user;
  (void)userdata;
  if (result != RC_OK) { _snprintf(message, sizeof(message), "Login failed: %s", error ? error : rc_error_str(result)); ra_message("RetroAchievements", message); return; }
  user = rc_client_get_user_info(client);
  WritePrivateProfileStringA("RetroAchievements", "Username", user->username, g_ra_ini);
  WritePrivateProfileStringA("RetroAchievements", "Token", user->token, g_ra_ini);
  WritePrivateProfileStringA("RetroAchievements", "Password", "", g_ra_ini);
  if (g_ra_rom && g_ra_rom_size)
    RA_OnRomLoaded(g_ra_rom, g_ra_rom_size);
}

void RA_Initialize(void) {
  char username[128], token[256], password[256];
  if (g_ra_client) return;
  GetModuleFileNameA(NULL, g_ra_ini, MAX_PATH);
  strcpy(strrchr(g_ra_ini, '\\') + 1, "1964-ra.ini");
  if (!GetPrivateProfileIntA("RetroAchievements", "Enabled", 1, g_ra_ini)) return;
  g_ra_http_idle = CreateEvent(NULL, TRUE, TRUE, NULL);
  g_ra_client = rc_client_create(ra_read_memory, ra_server_call);
  if (!g_ra_client) { CloseHandle(g_ra_http_idle); g_ra_http_idle = NULL; return; }
  rc_client_enable_logging(g_ra_client, RC_CLIENT_LOG_LEVEL_INFO, ra_log);
  rc_client_set_event_handler(g_ra_client, ra_event);
  /* This plugin cannot enforce all RetroAchievements Hardcore requirements.
     Keep it permanently in Casual mode, even if an old INI contains Hardcore=1. */
  rc_client_set_hardcore_enabled(g_ra_client, 0);
  WritePrivateProfileStringA("RetroAchievements", "Hardcore", "0", g_ra_ini);
  GetPrivateProfileStringA("RetroAchievements", "Username", "", username, sizeof(username), g_ra_ini);
  GetPrivateProfileStringA("RetroAchievements", "Token", "", token, sizeof(token), g_ra_ini);
  GetPrivateProfileStringA("RetroAchievements", "Password", "", password, sizeof(password), g_ra_ini);
  if (username[0] && token[0]) rc_client_begin_login_with_token(g_ra_client, username, token, ra_login_callback, NULL);
  else if (username[0] && password[0]) rc_client_begin_login_with_password(g_ra_client, username, password, ra_login_callback, NULL);
  else ra_message("RetroAchievements Setup", "Edit 1964-ra.ini and add your Username and Password, then restart 1964.");
}

void RA_OnRomLoaded(const unsigned char* rom, size_t rom_size) {
  g_ra_rom = rom; g_ra_rom_size = rom_size;
  if (g_ra_client && rc_client_get_user_info(g_ra_client))
    rc_client_begin_identify_and_load_game(g_ra_client, RC_CONSOLE_NINTENDO_64, NULL, rom, rom_size, ra_load_callback, NULL);
}
void RA_OnRomClosed(void) { g_ra_rom = NULL; g_ra_rom_size = 0; if (g_ra_client) rc_client_unload_game(g_ra_client); }
void RA_SetMemoryTable(const unsigned char** table) {
  if (table && table != g_ra_lut)
    ra_log_line("1964 RDRAM page table attached (raw word-swapped N64 layout)");
  g_ra_lut = table;
}
void RA_DoFrame(void) {
  if (g_ra_client && rc_client_get_game_info(g_ra_client)) {
    char message[160];
    rc_client_do_frame(g_ra_client);
    ++g_ra_frames;
    if (!g_ra_evaluation_logged) {
      _snprintf(message, sizeof(message),
                "Achievement evaluation started (memory read batches=%ld)", g_ra_memory_reads);
      ra_log_line(message);
      g_ra_evaluation_logged = 1;
    } else if ((g_ra_frames % 3600) == 0) {
      _snprintf(message, sizeof(message),
                "Achievement evaluation healthy: %lu frames, %ld memory read batches",
                g_ra_frames, g_ra_memory_reads);
      ra_log_line(message);
      rc_client_idle(g_ra_client);
    }
  }
}
void RA_Idle(void) { if (g_ra_client) rc_client_idle(g_ra_client); }
void RA_OnLoadState(void) { if (g_ra_client && rc_client_get_hardcore_enabled(g_ra_client)) { rc_client_set_hardcore_enabled(g_ra_client, 0); ra_message("RetroAchievements", "Hardcore disabled because a save state was loaded."); } }
void RA_Shutdown(void) {
  if (!g_ra_client) return;
  g_ra_shutting_down = 1;
  if (g_ra_http_count) WaitForSingleObject(g_ra_http_idle, 5000);
  rc_client_destroy(g_ra_client); g_ra_client = NULL;
  if (g_ra_http_idle) CloseHandle(g_ra_http_idle); g_ra_http_idle = NULL;
}
