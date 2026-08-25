# Draft RetroAchievements review request

Hello,

I have built an open-source, experimental RetroAchievements proxy for the 1964 GEPD Edition, intended for GoldenEye 007 and Perfect Dark. The source repository is:

https://github.com/djr296/RA-Experimental-support-for-1964-Emulator

The integration uses rcheevos v12.4.0 and identifies itself as:

`1964GEPD-RA-Proxy/v0.1.0 (Windows) rcheevos/v12.4.0`

Hardcore is permanently disabled in code. The 1964 input-plugin interface exposes ROM and RDRAM pointers but not a true video-interrupt callback, so achievement evaluation is driven by controller polling and capped at approximately 60 evaluations per second. The project is therefore being presented only as an experimental Casual-mode client.

GoldenEye 007 and Perfect Dark game identification and achievement triggering have been tested successfully. The repository includes full source, the GPL and MIT notices, build instructions, privacy disclosure, limitations, and no ROMs, emulator executable, original Mouse Injector binary, or game assets.

Before broadly distributing the binary, may I request your review and guidance on whether a public Casual-only beta is acceptable, and whether you require any changes to the client identity, integration behavior, or release wording?

Thank you.

