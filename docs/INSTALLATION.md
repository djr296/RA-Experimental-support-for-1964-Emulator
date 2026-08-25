# Installation and updating

This guide assumes you already have a clean copy of the official 1964 GEPD Edition and your own legally obtained GoldenEye 007 or Perfect Dark ROM.

## Before changing files

1. Close 1964 completely.
2. Copy the whole 1964 folder somewhere safe as a backup.
3. Confirm you are using the original `1964.exe` from the upstream bundle—not an older experimental `1964-RA.exe`.

## Files from this project

The Windows release contains:

- `Mouse_Injector_RA.dll` — the RetroAchievements proxy
- `1964-ra.ini.example` — a blank login template
- README, privacy information, licenses, and notices

It does not contain 1964, the original Mouse Injector, a ROM, or any game asset.

## Install the proxy

Suppose your emulator folder looks like this:

```text
1964 GEPD Edition\
  1964.exe
  plugin\
    Mouse_Injector.dll
```

Make these changes:

```text
1964 GEPD Edition\
  1964.exe
  1964-ra.ini
  plugin\
    Mouse_Injector_og.dll
    Mouse_Injector_RA.dll
```

Step by step:

1. Open the emulator's `plugin` folder.
2. Rename `Mouse_Injector.dll` to `Mouse_Injector_og.dll`.
3. Copy the downloaded `Mouse_Injector_RA.dll` into the `plugin` folder.
4. Copy `1964-ra.ini.example` into the main folder beside `1964.exe`.
5. Rename the copied file to `1964-ra.ini`.

The `_og` filename is required: the RA proxy loads that original DLL and forwards normal mouse/controller input to it.

## Sign in

Open `1964-ra.ini` with Notepad. It should resemble:

```ini
[RetroAchievements]
Enabled=1
Hardcore=0
Username=YOUR_USERNAME
Password=YOUR_PASSWORD
Token=
```

Enter your RetroAchievements username and password, save the file, and close Notepad. On the first successful login, the plugin clears `Password` and saves a token. The INI is plain text, so never upload or share the completed file.

## First launch

1. Start the original `1964.exe`.
2. If 1964 asks for an input plugin, choose the Mouse Injector entry ending in `+ RetroAchievements`.
3. Launch GoldenEye 007 or Perfect Dark.
4. Wait for the Casual-mode achievement message.
5. Earn an achievement you do not already have and confirm it appears on your RetroAchievements profile.

If mouse input is missing, the most likely cause is that `Mouse_Injector_og.dll` is absent or named differently.

## Updating

1. Close 1964.
2. Back up the current `plugin/Mouse_Injector_RA.dll`.
3. Replace it with the newer release DLL.
4. Keep `Mouse_Injector_og.dll` and your existing `1964-ra.ini` unchanged.

Read the release notes before updating in case a future version changes these steps.

## Uninstalling

1. Close 1964.
2. Delete `plugin/Mouse_Injector_RA.dll`.
3. Rename `plugin/Mouse_Injector_og.dll` back to `plugin/Mouse_Injector.dll`.
4. Optionally delete `1964-ra.ini` to remove your saved token.
5. Optionally delete `retroachievements.log` to remove the local diagnostic history.

