# 1964GEPD RA Proxy

An **unofficial, experimental, Casual-mode-only** RetroAchievements input-plugin proxy for the [1964 GEPD Edition](https://github.com/Graslu/1964GEPD). It preserves the existing Mouse Injector while adding achievement evaluation through the official [rcheevos](https://github.com/RetroAchievements/rcheevos) library.

This project is not affiliated with or endorsed by RetroAchievements, Nintendo, Rare, Microsoft, MGM, EON Productions, the 1964 developers, or the Mouse Injector authors.

## Important limitations

- Casual mode only. Hardcore is forcibly disabled in code.
- Experimental and not currently listed as an approved RetroAchievements emulator.
- Intended only for GoldenEye 007 and Perfect Dark with ROM hashes already supported by RetroAchievements.
- 1964's input-plugin API does not expose a true video-frame callback. Achievement evaluation is therefore driven by controller polling and capped at approximately 60 evaluations per second.
- No ROMs, game files, emulator executable, original Mouse Injector binary, or copyrighted artwork are included.

## Install

1. Download a clean copy of the official [1964 GEPD bundle](https://github.com/Graslu/1964GEPD/releases/tag/latest). Do not download ROMs from this project.
2. Close 1964.
3. Open the bundle's `plugin` folder.
4. Rename `Mouse_Injector.dll` to `Mouse_Injector_og.dll`. Keep it in that folder.
5. Copy `dist/Mouse_Injector_RA.dll` into the same `plugin` folder.
6. Copy `dist/1964-ra.ini.example` beside `1964.exe`, rename it to `1964-ra.ini`, and enter your RetroAchievements username and password.
7. Start the original `1964.exe`. In the input-plugin selection, choose the entry ending in `+ RetroAchievements` if it is not already selected.
8. Start a supported GoldenEye 007 or Perfect Dark ROM. A message should say that achievements are active in Casual mode.

After the first successful login, the password is removed from the INI and replaced with the account token returned by RetroAchievements. Treat that token like a password and never include your real INI in screenshots, bug reports, or uploads.

## Uninstall

1. Close 1964.
2. Delete `plugin/Mouse_Injector_RA.dll`.
3. Rename `plugin/Mouse_Injector_og.dll` back to `plugin/Mouse_Injector.dll`.
4. Delete `1964-ra.ini` and `retroachievements.log` if you also want to remove the saved login and local log.

## Build from source

Requirements: Visual Studio Build Tools with C/C++, CMake, Ninja, and the x86 build environment.

```bat
git clone --branch v12.4.0 https://github.com/RetroAchievements/rcheevos.git external\rcheevos
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DRCHEEVOS_DIR=external\rcheevos
cmake --build build --parallel
```

The 32-bit DLL is written to `dist/Mouse_Injector_RA.dll`.

## Source versions

- 1964 GEPD interface inspected at commit `9552acab919945c5f277da5efd146321e44a17bd`
- rcheevos v12.4.0, commit `2ad0b8672f68a48148620164510b963039e49eb1`

## License

The proxy source is licensed under GPL-2.0-or-later. rcheevos is MIT licensed. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md), [PRIVACY.md](PRIVACY.md), and the `licenses` directory.
