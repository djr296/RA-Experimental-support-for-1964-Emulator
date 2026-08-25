/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <windows.h>
#include <string.h>
#include "proxy_ra.h"

#define EXPORT __declspec(dllexport)
#define CALL __cdecl

typedef struct { WORD Version, Type; char Name[100]; BOOL Reserved1, Reserved2; } PLUGIN_INFO;
typedef struct { BOOL Present, RawData; int Plugin; } CONTROL;
typedef union { DWORD Value; } BUTTONS;

static HMODULE real_dll;
extern IMAGE_DOS_HEADER __ImageBase;
static ULONGLONG last_ra_frame_ms;
#define FP(name, ret, args) static ret (CALL *p_##name) args
FP(CloseDLL,void,(void)); FP(ControllerCommand,void,(int,BYTE*)); FP(DllAbout,void,(HWND));
FP(DllConfig,void,(HWND)); FP(DllTest,void,(HWND)); FP(GetDllInfo,void,(PLUGIN_INFO*));
FP(GetKeys,void,(int,BUTTONS*)); FP(InitiateControllers,void,(HWND,CONTROL*));
FP(ReadController,void,(int,BYTE*)); FP(RomClosed,void,(void)); FP(RomOpen,void,(void));
FP(WM_KeyDown,void,(WPARAM,LPARAM)); FP(WM_KeyUp,void,(WPARAM,LPARAM));
FP(HookRDRAM,void,(DWORD*,int)); FP(HookROM,void,(DWORD*));

static void load_real(void) {
  static const char* candidates[] = { "Mouse_Injector_og.dll", "Mouse_Injector_Original.dll", "Mouse_Injector.dll" };
  char path[MAX_PATH]; char* slash; int i;
  if (real_dll) return;
  GetModuleFileNameA((HMODULE)&__ImageBase, path, MAX_PATH);
  slash = strrchr(path, '\\');
  for (i = 0; i < 3 && !real_dll; ++i) { strcpy(slash + 1, candidates[i]); real_dll = LoadLibraryA(path); }
#define LOAD(name) p_##name = (void*)GetProcAddress(real_dll, #name)
  if (real_dll) { LOAD(CloseDLL); LOAD(ControllerCommand); LOAD(DllAbout); LOAD(DllConfig); LOAD(DllTest);
    LOAD(GetDllInfo); LOAD(GetKeys); LOAD(InitiateControllers); LOAD(ReadController); LOAD(RomClosed);
    LOAD(RomOpen); LOAD(WM_KeyDown); LOAD(WM_KeyUp); LOAD(HookRDRAM); LOAD(HookROM); }
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
  (void)reason; (void)reserved; DisableThreadLibraryCalls(instance);
  return TRUE;
}

EXPORT void CALL CloseDLL(void){RA_Shutdown(); if(p_CloseDLL)p_CloseDLL();}
EXPORT void CALL ControllerCommand(int c,BYTE* b){if(p_ControllerCommand)p_ControllerCommand(c,b);}
EXPORT void CALL DllAbout(HWND w){if(p_DllAbout)p_DllAbout(w);}
EXPORT void CALL DllConfig(HWND w){if(p_DllConfig)p_DllConfig(w);}
EXPORT void CALL DllTest(HWND w){if(p_DllTest)p_DllTest(w);}
EXPORT void CALL GetDllInfo(PLUGIN_INFO* i){
  static const char suffix[]=" + RetroAchievements";
  size_t used;
  load_real();
  if(!i) return;
  if(p_GetDllInfo) {
    p_GetDllInfo(i);
    i->Name[sizeof(i->Name)-1]=0;
    used=strlen(i->Name);
    strncat(i->Name,suffix,sizeof(i->Name)-used-1);
  } else {
    ZeroMemory(i,sizeof(*i));
    i->Version=0x0100;
    i->Type=4;
    strcpy(i->Name,"RA proxy: Mouse_Injector_og.dll not found");
  }
}
EXPORT void CALL GetKeys(int c,BUTTONS* k){
  if(p_GetKeys)p_GetKeys(c,k);
  if(c==0) {
    ULONGLONG now=GetTickCount64();
    /* PIF reads are not a frame boundary and can occur more than once per VI.
       Cap evaluation at approximately NTSC VI rate to protect emulation speed. */
    if(now-last_ra_frame_ms>=16) { last_ra_frame_ms=now; RA_DoFrame(); }
  }
}
EXPORT void CALL InitiateControllers(HWND w,CONTROL* c){load_real(); RA_Initialize(); if(p_InitiateControllers)p_InitiateControllers(w,c);}
EXPORT void CALL ReadController(int c,BYTE* b){if(p_ReadController)p_ReadController(c,b);}
EXPORT void CALL RomClosed(void){RA_OnRomClosed(); if(p_RomClosed)p_RomClosed();}
EXPORT void CALL RomOpen(void){if(p_RomOpen)p_RomOpen();}
EXPORT void CALL WM_KeyDown(WPARAM w,LPARAM l){if(p_WM_KeyDown)p_WM_KeyDown(w,l);}
EXPORT void CALL WM_KeyUp(WPARAM w,LPARAM l){if(p_WM_KeyUp)p_WM_KeyUp(w,l);}
EXPORT void CALL HookRDRAM(DWORD* m,int o){RA_SetMemoryTable((const unsigned char**)m); if(p_HookRDRAM)p_HookRDRAM(m,o);}
EXPORT void CALL HookROM(DWORD* r){
  char name[21]; int i; size_t size=0;
  for(i=0;i<20;i++) name[i]=(char)((unsigned char*)r)[(0x20+i)^3]; name[20]=0;
  if(strstr(name,"GOLDENEYE")) size=12582912; else if(strstr(name,"Perfect Dark")) size=33554432;
  if(size) RA_OnRomLoaded((const unsigned char*)r,size);
  if(p_HookROM)p_HookROM(r);
}
