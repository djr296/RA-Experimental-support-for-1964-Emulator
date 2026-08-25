# RetroAchievements for 1964 GEPD

[![Windows x86 build](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator/actions/workflows/build.yml/badge.svg)](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator/actions/workflows/build.yml)

Play **GoldenEye 007** and **Perfect Dark** in the mouse-and-keyboard-focused [1964 GEPD Edition](https://github.com/Graslu/1964GEPD) while earning RetroAchievements.

This small Windows plugin sits in front of the existing Mouse Injector. Mouse input, fullscreen, video settings, and the emulator itself still come from the normal 1964 GEPD bundle; the proxy adds game identification and achievement checking through the official [rcheevos](https://github.com/RetroAchievements/rcheevos) library.

> **Experimental Casual-mode release.** Hardcore is permanently disabled. This is an unofficial community project and is not currently listed as an approved RetroAchievements emulator.

## Download

Open [Releases](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator/releases), choose the newest version, and download the file ending in **`Windows-Casual.zip`**.

You will also need:

- Windows and the original 32-bit `1964.exe` from the [official 1964 GEPD bundle](https://github.com/Graslu/1964GEPD/releases/tag/latest)
- Your own legally obtained GoldenEye 007 or Perfect Dark ROM
- A free [RetroAchievements account](https://retroachievements.org/createaccount.php)

No ROM, emulator executable, game asset, or original Mouse Injector is included here.

## Install in about five minutes

1. Close 1964 and make a backup of its folder.
2. Open the emulator's `plugin` folder.
3. Rename `Mouse_Injector.dll` to **`Mouse_Injector_og.dll`**. Do not delete it.
4. Copy `Mouse_Injector_RA.dll` from the downloaded release into that same `plugin` folder.
5. Copy `1964-ra.ini.example` beside `1964.exe` and rename it to **`1964-ra.ini`**.
6. Open `1964-ra.ini` in Notepad and enter your RetroAchievements username and password. Leave `Hardcore=0`.
7. Start the original `1964.exe`. If prompted to choose an input plugin, select the Mouse Injector entry ending in **`+ RetroAchievements`**.
8. Start a supported game. You should see **“Achievements active for … (Casual mode)”**.

After the first successful login, the plugin erases the password from the INI and saves the RetroAchievements login token instead. Do not share your real `1964-ra.ini`—the token should be treated like a password.

For a slower file-by-file walkthrough, see [Installation and updating](docs/INSTALLATION.md).

## Does it award real achievements?

Yes. When an achievement condition triggers and RetroAchievements accepts it, the unlock is recorded as a **Casual** unlock on your RetroAchievements profile. The startup message only confirms that the game and achievement set loaded; it does not by itself prove that an achievement unlocked.

To verify everything:

1. Trigger an easy achievement you have not previously earned.
2. Look for the **Achievement Unlocked** message.
3. Refresh your profile on RetroAchievements and confirm the new Casual unlock appears.
4. If it does not, close the game and inspect `retroachievements.log` beside `1964.exe`.

## Supported games

| Game | Status | Notes |
| --- | --- | --- |
| GoldenEye 007 | Tested | Requires a ROM hash supported by the existing RetroAchievements set. |
| Perfect Dark | Tested | Requires a ROM hash supported by the existing RetroAchievements set. |

This proxy deliberately targets these two games. Other N64 games are not supported, and the specialized 1964 GEPD build is generally a poor choice for them.

## Important limitations

- **Casual mode only.** The emulator cannot enforce every RetroAchievements Hardcore rule, so the proxy always writes `Hardcore=0` and ignores attempts to enable it.
- **Windows x86 only.** Wine and other operating systems have not been validated.
- **Experimental client.** Do not describe this project as official or approved by RetroAchievements.
- **Controller-poll evaluation.** 1964's plugin interface has no true video-frame callback. The proxy evaluates from controller polling and caps checks at roughly 60 per second to protect game performance.
- **No bundled emulator or games.** Install over a clean upstream 1964 GEPD bundle and supply your own ROM.

## Problems?

Start with [Troubleshooting](docs/TROUBLESHOOTING.md). The most common fixes are:

- Keep both `Mouse_Injector_RA.dll` and `Mouse_Injector_og.dll` in the `plugin` folder.
- Put `1964-ra.ini` beside `1964.exe`, not in the plugin folder.
- Launch the original `1964.exe`; do not use an old experimental `1964-RA.exe`.
- Confirm your ROM hash is supported by RetroAchievements.
- Read `retroachievements.log` and remove private information before attaching an excerpt to a bug report.

## Update or remove it

To update, close 1964 and replace only `plugin/Mouse_Injector_RA.dll`. Keep your existing `1964-ra.ini` so your saved token is preserved.

To uninstall:

1. Delete `plugin/Mouse_Injector_RA.dll`.
2. Rename `plugin/Mouse_Injector_og.dll` back to `Mouse_Injector.dll`.
3. Optionally delete `1964-ra.ini` and `retroachievements.log` to remove the saved login and local log.

## For developers and reviewers

The repository contains the complete proxy source and a Windows x86 workflow that performs a clean build against pinned rcheevos v12.4.0. The workflow validates the DLL architecture but intentionally does **not** upload an Actions artifact, so it uses no artifact-storage quota.

See [Contributing](CONTRIBUTING.md), [Privacy](PRIVACY.md), [Security](SECURITY.md), and [third-party notices](THIRD_PARTY_NOTICES.md). The proxy is GPL-2.0-or-later; rcheevos is MIT licensed.

## Unofficial project notice

This project is not affiliated with or endorsed by RetroAchievements, Nintendo, Rare, Microsoft, MGM, EON Productions, the 1964 developers, or the Mouse Injector authors. Product and game names are used only to describe compatibility.

