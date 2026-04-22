#if defined AVE_ADMIN_PERMISSION_LEVEL_INC
    #endinput
#endif
#define AVE_ADMIN_PERMISSION_LEVEL_INC

enum E_ADMIN_LEVEL
{
    ADMIN_LEVEL_NONE = 0,
    ADMIN_LEVEL_HELPER,
    ADMIN_LEVEL_MODERATOR,
    ADMIN_LEVEL_SENIOR_MODERATOR,
    ADMIN_LEVEL_ADMINISTRATOR,
    ADMIN_LEVEL_SENIOR_ADMIN,
    ADMIN_LEVEL_HEAD_ADMIN,
    ADMIN_LEVEL_MANAGEMENT,
    ADMIN_LEVEL_DEVELOPER
}

// ====== Admin_GetLevelName ======
stock Admin_GetLevelName(level, dest[], size)
{
    switch (level)
    {
        case ADMIN_LEVEL_HELPER: format(dest, size, "Helper");
        case ADMIN_LEVEL_MODERATOR: format(dest, size, "Moderator");
        case ADMIN_LEVEL_SENIOR_MODERATOR: format(dest, size, "Senior Moderator");
        case ADMIN_LEVEL_ADMINISTRATOR: format(dest, size, "Administrator");
        case ADMIN_LEVEL_SENIOR_ADMIN: format(dest, size, "Senior Administrator");
        case ADMIN_LEVEL_HEAD_ADMIN: format(dest, size, "Head Administrator");
        case ADMIN_LEVEL_MANAGEMENT: format(dest, size, "Management");
        case ADMIN_LEVEL_DEVELOPER: format(dest, size, "Developer");
        default: format(dest, size, "None");
    }

    return 1;
}
