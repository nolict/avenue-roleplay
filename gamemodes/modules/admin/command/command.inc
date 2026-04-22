#if defined AVE_ADMIN_COMMAND_LOADER_INC
    #endinput
#endif
#define AVE_ADMIN_COMMAND_LOADER_INC

#include <zcmd>

// ====== Helper Level Commands ======

CMD:adminlevel(playerid, params[])
{
    new levelName[32];
    new message[96];

    #pragma unused params

    if (!AccountSession_IsAuthenticated(playerid))
    {
        SendErrorMessage(playerid, "You are not logged into an account.");
        return 1;
    }

    Admin_GetLevelName(_:AdminSession_GetLevel(playerid), levelName, sizeof(levelName));
    format(message, sizeof(message), "Your admin level is %s (%d).", levelName, _:AdminSession_GetLevel(playerid));
    Admin_SendServerText(playerid, message);
    return 1;
}

CMD:ahelp(playerid, params[])
{
    #pragma unused params

    if (!Admin_IsAuthorized(playerid, ADMIN_LEVEL_HELPER))
    {
        return 1;
    }

    return Admin_SendCommandList(playerid);
}

CMD:aduty(playerid, params[])
{
    #pragma unused params

    if (!Admin_IsAuthorized(playerid, ADMIN_LEVEL_HELPER))
    {
        return 1;
    }

    if (AdminSession_IsOnDuty(playerid))
    {
        AdminSession_SetDuty(playerid, false);
        Admin_SendAdminText(playerid, "Admin duty disabled.");
        return 1;
    }

    AdminSession_SetDuty(playerid, true);
    Admin_SendAdminText(playerid, "Admin duty enabled.");
    return 1;
}

// ====== Moderator Level Commands ======

CMD:spec(playerid, params[])
{
    new targetid;

    if (!Admin_IsDutyAuthorized(playerid, ADMIN_LEVEL_MODERATOR))
    {
        return 1;
    }

    if (!Admin_ParsePlayerTarget(playerid, params, "/spec [playerid]", targetid))
    {
        return 1;
    }

    if (targetid == playerid)
    {
        SendErrorMessage(playerid, "You cannot spectate yourself.");
        return 1;
    }

    TogglePlayerSpectating(playerid, true);
    PlayerSpectatePlayer(playerid, targetid);
    Admin_SendAdminText(playerid, "Now spectating target player.");
    return 1;
}

CMD:goto(playerid, params[])
{
    new targetid;
    new Float:x, Float:y, Float:z;

    if (!Admin_IsDutyAuthorized(playerid, ADMIN_LEVEL_MODERATOR))
    {
        return 1;
    }

    if (!Admin_ParsePlayerTarget(playerid, params, "/goto [playerid]", targetid))
    {
        return 1;
    }

    if (targetid == playerid)
    {
        SendErrorMessage(playerid, "You are already there.");
        return 1;
    }

    GetPlayerPos(targetid, x, y, z);
    SetPlayerPos(playerid, x, y, z);
    SetPlayerInterior(playerid, GetPlayerInterior(targetid));
    SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));
    Admin_SendAdminText(playerid, "Teleported to target player.");
    return 1;
}

CMD:gethere(playerid, params[])
{
    new targetid;
    new Float:x, Float:y, Float:z;

    if (!Admin_IsDutyAuthorized(playerid, ADMIN_LEVEL_MODERATOR))
    {
        return 1;
    }

    if (!Admin_ParsePlayerTarget(playerid, params, "/gethere [playerid]", targetid))
    {
        return 1;
    }

    if (targetid == playerid)
    {
        SendErrorMessage(playerid, "You cannot teleport yourself.");
        return 1;
    }

    GetPlayerPos(playerid, x, y, z);
    SetPlayerPos(targetid, x, y, z);
    SetPlayerInterior(targetid, GetPlayerInterior(playerid));
    SetPlayerVirtualWorld(targetid, GetPlayerVirtualWorld(playerid));
    Admin_SendAdminText(playerid, "Target player teleported to you.");
    SendServerMessage(targetid, "An administrator has teleported you.");
    return 1;
}

// ====== Head Administrator Level Commands ======

CMD:setadmin(playerid, params[])
{
    new targetid;
    new level;
    new targetName[MAX_PLAYER_NAME + 1];
    new targetLevelName[32];
    new message[144];

    if (!Admin_IsAuthorized(playerid, ADMIN_LEVEL_HEAD_ADMIN))
    {
        return 1;
    }

    if (!Admin_ParseSetAdminParams(playerid, params, targetid, level))
    {
        return 1;
    }

    if (targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid))
    {
        SendErrorMessage(playerid, "Invalid target player.");
        return 1;
    }

    if (targetid == playerid)
    {
        SendErrorMessage(playerid, "You cannot change your own admin level.");
        return 1;
    }

    if (!AccountSession_IsAuthenticated(targetid))
    {
        SendErrorMessage(playerid, "Target player is not logged into an account.");
        return 1;
    }

    if (level < _:ADMIN_LEVEL_NONE || level > _:ADMIN_LEVEL_DEVELOPER)
    {
        SendErrorMessage(playerid, "Invalid admin level.");
        return 1;
    }

    if (AdminSession_GetLevel(playerid) <= AdminSession_GetLevel(targetid))
    {
        SendErrorMessage(playerid, "You cannot edit an equal or higher admin level.");
        return 1;
    }

    if (_:AdminSession_GetLevel(playerid) <= level)
    {
        SendErrorMessage(playerid, "You cannot assign a level equal to or above your own.");
        return 1;
    }

    Admin_SetAccountLevel(AccountSession_GetId(targetid), level, AccountSession_GetId(playerid));
    AdminSession_SetLevel(targetid, E_ADMIN_LEVEL:level);

    GetPlayerName(targetid, targetName, sizeof(targetName));
    Admin_GetLevelName(level, targetLevelName, sizeof(targetLevelName));

    format(message, sizeof(message), "%s is now %s.", targetName, targetLevelName);
    Admin_SendAdminText(playerid, message);

    format(message, sizeof(message), "Your admin level has been updated to %s.", targetLevelName);
    Admin_SendServerText(targetid, message);

    return 1;
}

// ====== Developer Level Commands ======

CMD:setinterior(playerid, params[])
{
    new interiorWorldId;
    new variantId;

    if (!Admin_IsDutyAuthorized(playerid, ADMIN_LEVEL_DEVELOPER))
    {
        return 1;
    }

    if (!Admin_ParseInteriorId(playerid, params, interiorWorldId, variantId))
    {
        return 1;
    }

    Admin_TeleportToInterior(playerid, interiorWorldId, variantId);
    return 1;
}
