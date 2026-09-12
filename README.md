# RetroAchievements for GoldenEye 007 & Perfect Dark on 1964 GEPD

Earn RetroAchievements while playing **GoldenEye 007** and **Perfect Dark** with the mouse-and-keyboard-focused [1964 GEPD Edition](https://github.com/Graslu/1964GEPD).

> **Unofficial experimental release — Casual mode only.** This project is not an official or approved RetroAchievements emulator, and Hardcore mode is not supported.

## What you need

- Windows
- A clean copy of the [latest 1964 GEPD Edition](https://github.com/Graslu/1964GEPD/releases/tag/latest)
- Your own legally obtained GoldenEye 007 or Perfect Dark ROM
- A free [RetroAchievements account](https://retroachievements.org/createaccount.php)

This download does **not** include ROMs, game assets, or the emulator itself.

## Download

[**Download version 0.1.0 for Windows**](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator/releases/download/v0.1.0/1964GEPD-RA-Proxy-v0.1.0-Windows-Casual.zip)

You can also visit the [Releases page](https://github.com/djr296/RA-Experimental-support-for-1964-Emulator/releases) to check for a newer version. Download the ZIP whose name ends in **`Windows-Casual.zip`**.

## Installation

Start with a fresh, working 1964 GEPD folder.

1. Close 1964.
2. Make a backup copy of your 1964 GEPD folder.
3. Extract the downloaded RetroAchievements ZIP.
4. Open the **`plugin`** folder inside your 1964 GEPD folder.
5. Rename **`Mouse_Injector.dll`** to **`Mouse_Injector_og.dll`**.
   - Keep this file. The RetroAchievements plugin uses it to preserve the normal mouse injector.
6. Copy **`Mouse_Injector_RA.dll`** from the downloaded ZIP into the **`plugin`** folder.
7. Copy **`1964-ra.ini.example`** from the ZIP into the main 1964 GEPD folder, beside **`1964.exe`**.
8. Rename **`1964-ra.ini.example`** to **`1964-ra.ini`**.
9. Open **`1964-ra.ini`** in Notepad and enter your RetroAchievements username and password:

```ini
[RetroAchievements]
Enabled=1
Hardcore=0
Username=YourUsername
Password=YourPassword
Token=
```

10. Save the file and start the original **`1964.exe`**.
11. If 1964 asks you to choose an input plugin, select the Mouse Injector entry ending in **`+ RetroAchievements`**.
12. Start GoldenEye 007 or Perfect Dark.

Your files should now look like this:

```text
1964 GEPD/
├── 1964.exe
├── 1964-ra.ini
└── plugin/
    ├── Mouse_Injector_og.dll
    └── Mouse_Injector_RA.dll
```

When a supported game starts, you should see:

```text
Achievements active for ... (Casual mode)
```

After your first successful login, the plugin removes the password from the INI and saves a login token instead. **Do not share your `1964-ra.ini` file**, because that token should be treated like a password.

For a more detailed walkthrough, see [Installation and updating](docs/INSTALLATION.md).

## Confirm achievements are working

The startup message means your game and its achievement set loaded. To confirm that unlocks are registering:

1. Choose an achievement you have not already earned.
2. Complete its requirements.
3. Look for the **Achievement Unlocked** message.
4. Refresh your RetroAchievements profile and confirm that it appears as a Casual unlock.

If the achievement does not appear, close 1964 and check **`retroachievements.log`** beside **`1964.exe`**.

## Common problems

**No RetroAchievements message appears**

- Confirm that both DLL files are in the **`plugin`** folder.
- Confirm that **`1964-ra.ini`** is beside **`1964.exe`**, not inside the plugin folder.
- Make sure 1964 is using the input plugin ending in **`+ RetroAchievements`**.
- Make sure your exact ROM version is supported by RetroAchievements.

**The mouse injector does not work**

- The original plugin must be named exactly **`Mouse_Injector_og.dll`**.
- Do not replace it with a second copy of **`Mouse_Injector_RA.dll`**.

**The game is black, crashes, is pixelated, or runs slowly**

- Use a fresh official 1964 GEPD folder.
- Launch the original **`1964.exe`**—do not use an old experimental **`1964-RA.exe`**.
- Restore the video, audio, and emulator settings supplied with the official 1964 GEPD bundle.

**Login fails**

- Recheck your username and password in **`1964-ra.ini`**.
- If a saved token is no longer valid, clear the **`Token=`** value, enter your password again, and restart 1964.

See [Troubleshooting](docs/TROUBLESHOOTING.md) for more help. Remove usernames, tokens, passwords, and other private information before sharing a log.

## Updating

1. Close 1964.
2. Download the newest release.
3. Replace only **`plugin/Mouse_Injector_RA.dll`** with the new version.
4. Keep your existing **`1964-ra.ini`** so your saved login token is preserved.

## Uninstalling

1. Close 1964.
2. Delete **`plugin/Mouse_Injector_RA.dll`**.
3. Rename **`plugin/Mouse_Injector_og.dll`** back to **`Mouse_Injector.dll`**.
4. If you also want to remove your saved login, delete **`1964-ra.ini`** and **`retroachievements.log`**.

## Compatibility and limitations

- GoldenEye 007 and Perfect Dark only
- Windows x86 only
- Casual achievements only; Hardcore cannot be enabled
- A RetroAchievements-supported ROM hash is required
- Other N64 games are not supported
- This is experimental community software

## More information

- [Privacy and account data](PRIVACY.md)
- [Security policy](SECURITY.md)
- [License](LICENSE)
- [Third-party notices](THIRD_PARTY_NOTICES.md)

This project is not affiliated with or endorsed by RetroAchievements, Nintendo, Rare, Microsoft, MGM, EON Productions, the 1964 developers, or the Mouse Injector authors. Product and game names are used only to describe compatibility.
