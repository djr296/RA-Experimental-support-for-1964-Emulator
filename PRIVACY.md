# Privacy

Version 0.1.0 does not contain analytics, advertising, crash reporting, or project-operated servers.

To provide achievements, the plugin sends login, game-identification, achievement-unlock, and normal rcheevos client requests directly to RetroAchievements over HTTPS. RetroAchievements controls its own server-side processing and retention; users should review the RetroAchievements privacy policy and terms before signing in.

The plugin creates these local files beside `1964.exe`:

- `1964-ra.ini`: enabled state, RetroAchievements username, and login token. A password entered for first login is erased after a successful login and replaced by the returned token. The file is plain text and is not encrypted.
- `retroachievements.log`: local diagnostic messages that can include game titles, achievement IDs and titles, and connection/evaluation status.

The project maintainers do not receive these files automatically. Users should remove account tokens and personal paths before posting logs or configuration files publicly. Local files remain until the user deletes them.

