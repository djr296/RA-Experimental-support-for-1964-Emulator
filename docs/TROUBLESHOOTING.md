# Troubleshooting

## No RetroAchievements startup message

Check all of these:

- `Mouse_Injector_RA.dll` is inside the emulator's `plugin` folder.
- `Mouse_Injector_og.dll` is beside it with that exact filename.
- `1964-ra.ini` is beside `1964.exe`, not inside `plugin`.
- `Enabled=1`, your username is correct, and either `Password` or `Token` has a value.
- You launched the original `1964.exe` while connected to the internet.
- Your ROM hash is supported by the existing RetroAchievements game page.

Then inspect `retroachievements.log` beside `1964.exe` for login or game-identification errors.

## Account connects, but no achievement unlocks

The startup banner only proves that the game and set loaded. It does not prove that a particular condition triggered.

1. Make sure you are testing an achievement you have not already earned.
2. Read its exact requirements and avoid using a save state that skips required progression.
3. Complete the requirement and wait several seconds.
4. Refresh your RetroAchievements profile and check Casual unlocks.
5. Look in `retroachievements.log` for `Achievement evaluation started` or `Achievement triggered`.

When reporting a problem, include the game, proxy version, achievement name, exact reproduction steps, and a short sanitized log excerpt. Never attach your ROM or `1964-ra.ini`.

## Mouse and keyboard no longer work

The proxy forwards input to the original Mouse Injector. Confirm this exact pair exists:

```text
plugin\Mouse_Injector_RA.dll
plugin\Mouse_Injector_og.dll
```

If only `Mouse_Injector.dll` exists, rename it to `Mouse_Injector_og.dll`. If the original DLL was deleted, restore it from a clean official 1964 GEPD download.

## Game is slow or looks pixelated

The RA proxy does not replace the video plugin or emulator core. First test the clean emulator configuration:

1. Close 1964.
2. Temporarily remove `Mouse_Injector_RA.dll`.
3. Rename `Mouse_Injector_og.dll` back to `Mouse_Injector.dll`.
4. Test the game again.

If it is still slow, the cause is the emulator/video configuration rather than RetroAchievements. Restore the video plugin and settings supplied by the official 1964 GEPD bundle. Avoid copying old `1964.cfg` files from unrelated builds.

If the clean setup is fast but the proxy setup is slow, restore the proxy, reproduce the problem, and report the newest sanitized log.

## Black screen or crash

- Start with a fresh official 1964 GEPD bundle.
- Use its original `1964.exe`; do not use an old standalone `1964-RA.exe`.
- Do not mix video, audio, or RSP plugins from other 1964 or Project64 packages.
- Confirm the ROM boots before installing this proxy.
- Close 1964 before replacing DLLs.

GoldenEye 007 and Perfect Dark should both run normally before RetroAchievements is added.

## Fullscreen does not work

Fullscreen is controlled by 1964 and its video plugin, not this input proxy. Verify fullscreen on the clean official bundle first, then install the RA DLL without replacing `1964.cfg` or video plugins.

## Login fails repeatedly

1. Close 1964.
2. Open `1964-ra.ini`.
3. Clear the `Token=` value.
4. Re-enter the correct username and password.
5. Save the file and restart 1964.

Never post the failed-login INI publicly. If a token was exposed, treat it as compromised and replace it.

## Windows blocks or warns about the DLL

The DLL is currently unsigned, so Windows may identify it as coming from an unknown publisher. Download it only from this repository's Releases page and compare its SHA-256 value with the release notes. Do not download reposted copies from ROM or file-sharing sites.

## Reporting a bug safely

Use the repository's bug-report form. Before submitting:

- Remove usernames, passwords, tokens, personal paths, and ROM filenames from logs.
- Do not upload ROMs, saves containing copyrighted game data, screenshots of credentials, or a full emulator bundle.
- State that you are using Casual mode and provide the proxy version.

