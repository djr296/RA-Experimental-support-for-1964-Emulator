# v0.1.0 — First experimental Casual-mode release

This is the first public test release of RetroAchievements support for **GoldenEye 007** and **Perfect Dark** in the **1964 GEPD Edition**. It keeps the normal 1964 GEPD mouse injector, fullscreen support, and game-performance setup while adding real RetroAchievements Casual unlocks.

## Download

Download **`1964GEPD-RA-Proxy-v0.1.0-Windows-Casual.zip`** from the Assets section below.

Do **not** download GitHub's automatically generated “Source code” ZIP or TAR files unless you want to compile the project yourself.

## Included files

- **`Mouse_Injector_RA.dll`** — the RetroAchievements-enabled input-plugin proxy
- **`1964-ra.ini.example`** — a safe configuration template with no account information

The 1964 emulator, original Mouse Injector, and game ROMs are not included.

## Quick installation

1. Start with a clean, working copy of the [official 1964 GEPD Edition](https://github.com/Graslu/1964GEPD/releases/tag/latest), then close the emulator.
2. Open its **`plugin`** folder.
3. Rename **`Mouse_Injector.dll`** to **`Mouse_Injector_og.dll`**.
4. Copy **`Mouse_Injector_RA.dll`** into the **`plugin`** folder.
5. Copy **`1964-ra.ini.example`** beside **`1964.exe`** and rename it to **`1964-ra.ini`**.
6. Open the INI in Notepad and add your RetroAchievements username and password. Leave **`Hardcore=0`**.
7. Start the original **`1964.exe`** and select the input plugin ending in **`+ RetroAchievements`** if needed.
8. Launch GoldenEye 007 or Perfect Dark.

After the first successful login, the password is removed from the INI and replaced with a login token. Never share your real **`1964-ra.ini`**.

[Read the complete installation guide](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator#installation)

## What works

- GoldenEye 007 and Perfect Dark game identification
- Casual achievement evaluation and server submission
- On-screen game-loaded and achievement-unlocked messages
- Existing 1964 GEPD Mouse Injector controls
- Fullscreen and the normal 1964 GEPD video/audio configuration
- Saved RetroAchievements login token after the first successful login

## Important notes

- **Casual mode only.** Hardcore mode cannot be enabled.
- This is an unofficial, experimental community client and is not currently listed as an approved RetroAchievements emulator.
- A ROM hash supported by the game's RetroAchievements set is required.
- Windows x86 is the supported platform.
- The DLL is unsigned, so Windows may display an unfamiliar-publisher warning.

The “Achievements active” startup message confirms that the achievement set loaded. To fully verify the integration, unlock an achievement you have not previously earned and confirm that the Casual unlock appears on your RetroAchievements profile.

For help, see the [troubleshooting guide](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator/blob/main/docs/TROUBLESHOOTING.md).

## Verification

Release ZIP SHA-256:

`B0624F3D6D703CFEE56F9C2FAE08405F903C4F0D30CC59879AF2CA77DF885337`

No ROMs, emulator executable, original Mouse Injector binary, game code, artwork, music, or other copyrighted game assets are included.
