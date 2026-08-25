# v0.1.0 — First experimental Casual-mode release

This is the first public test release of **RetroAchievements support for the 1964 GEPD Edition**. It lets you keep the specialized GoldenEye 007/Perfect Dark mouse controls and performance tweaks while earning real RetroAchievements Casual unlocks.

## Download

Download **`1964GEPD-RA-Proxy-v0.1.0-Windows-Casual.zip`** below. Do not download GitHub's automatically generated “Source code” archives unless you intend to compile the project yourself.

## What works

- GoldenEye 007 and Perfect Dark game identification
- Casual achievement evaluation and server submission
- On-screen game-loaded and achievement-unlocked messages
- Existing 1964 GEPD Mouse Injector controls through the original plugin
- Saved RetroAchievements login token after the first successful login
- Fullscreen and normal 1964 GEPD video/audio configuration

## Quick installation

1. Install a clean copy of the [official 1964 GEPD bundle](https://github.com/Graslu/1964GEPD/releases/tag/latest), then close the emulator.
2. In its `plugin` folder, rename `Mouse_Injector.dll` to `Mouse_Injector_og.dll`.
3. Copy the release's `Mouse_Injector_RA.dll` into that `plugin` folder.
4. Copy `1964-ra.ini.example` beside `1964.exe`, rename it to `1964-ra.ini`, and add your RetroAchievements username and password.
5. Start the original `1964.exe`, select the input plugin ending in `+ RetroAchievements` if necessary, and launch a supported game.

The password is erased after a successful login and replaced with a token. Never share your real `1964-ra.ini`.

Full instructions: [README](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator#install-in-about-five-minutes)

## What “experimental” means

- **Casual mode only.** Hardcore is forcibly disabled because this plugin cannot enforce all RetroAchievements Hardcore restrictions.
- This is an unofficial community client and is not currently listed as an approved RetroAchievements emulator.
- Achievement evaluation uses controller polling rather than a true emulator frame callback and is capped at roughly 60 checks per second.
- Only GoldenEye 007 and Perfect Dark have been tested.
- Windows x86 is the supported platform; Wine is untested.

## Confirming an unlock

The “Achievements active” startup message confirms that the game set loaded. For a complete test, earn an achievement you do not already have, look for the unlock popup, and verify that the Casual unlock appears on your RetroAchievements profile.

If something goes wrong, follow the [troubleshooting guide](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator/blob/main/docs/TROUBLESHOOTING.md) and sanitize logs before posting them.

## Files and verification

`Mouse_Injector_RA.dll` SHA-256:

`11A758B588751EF3863802516A3FFC42292DC4365D2E0E03E117453E5DEF76F2`

The DLL is unsigned, so Windows may show an unfamiliar-publisher warning. The repository contains its complete matching source and an automated clean Windows x86 build.

No ROMs, emulator executable, original Mouse Injector binary, game code, artwork, music, or other game assets are included.

