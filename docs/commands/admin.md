# Admin Commands

This document lists the current admin commands implemented in Avenue Roleplay.

## Command Organization

All admin commands live in:

```text
gamemodes/modules/admin/command/command.inc
```

For consistency, commands are grouped inside that file by comment headers that represent the required admin level. New commands should follow the same pattern.

## Admin Ladder

| Level | Name |
| --- | --- |
| 0 | None |
| 1 | Helper |
| 2 | Moderator |
| 3 | Senior Moderator |
| 4 | Administrator |
| 5 | Senior Administrator |
| 6 | Head Administrator |
| 7 | Management |
| 8 | Developer |

## Command Reference

| Command | Minimum Level | Description | Example |
| --- | --- | --- | --- |
| `/adminlevel` | None, but requires authenticated account | Shows the active admin level currently loaded into the runtime session. Useful for verifying whether admin data has been loaded correctly after login. | `/adminlevel` |
| `/ahelp` | Helper | Displays the admin command list available to the current admin level. | `/ahelp` |
| `/spec [playerid]` | Moderator | Starts spectating the target player. The command rejects self-spectate and invalid targets. | `/spec 3` |
| `/goto [playerid]` | Moderator | Teleports the admin to the target player and synchronizes interior and virtual world with the target. | `/goto 8` |
| `/gethere [playerid]` | Moderator | Teleports the target player to the admin and synchronizes interior and virtual world with the admin. | `/gethere 12` |
| `/setadmin [playerid] [level]` | Head Administrator | Assigns or updates the target account's admin level. The command rejects invalid targets, invalid levels, and attempts to modify equal or higher authority. | `/setadmin 7 2` |

## Notes

- Admin roles are owned by account, not character.
- The `admin_accounts` table is the source of truth for persistent admin level data.
- The runtime session is refreshed after successful account authentication.
- The first admin can be bootstrapped manually with the commented SQL snippet inside `database/schema.sql`.
