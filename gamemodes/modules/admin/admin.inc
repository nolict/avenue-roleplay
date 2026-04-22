#if defined AVE_ADMIN_LOADER_INC
    #endinput
#endif
#define AVE_ADMIN_LOADER_INC

#include <a_mysql>

#include "modules/admin/permission/level.inc"

forward OnAdminAccountLevelLoaded(playerid, accountId);

new E_ADMIN_LEVEL:g_AdminLevel[MAX_PLAYERS];
new bool:g_AdminDuty[MAX_PLAYERS];

// ====== AdminSession_Reset ======
stock AdminSession_Reset(playerid)
{
    g_AdminLevel[playerid] = ADMIN_LEVEL_NONE;
    g_AdminDuty[playerid] = false;
    return 1;
}

// ====== AdminSession_SetLevel ======
stock AdminSession_SetLevel(playerid, E_ADMIN_LEVEL:level)
{
    g_AdminLevel[playerid] = level;

    if (level == ADMIN_LEVEL_NONE)
    {
        g_AdminDuty[playerid] = false;
    }

    return 1;
}

// ====== AdminSession_GetLevel ======
stock E_ADMIN_LEVEL:AdminSession_GetLevel(playerid)
{
    return g_AdminLevel[playerid];
}

// ====== AdminSession_SetDuty ======
stock AdminSession_SetDuty(playerid, bool:isDuty)
{
    g_AdminDuty[playerid] = isDuty;
    return 1;
}

// ====== AdminSession_IsOnDuty ======
stock bool:AdminSession_IsOnDuty(playerid)
{
    return g_AdminDuty[playerid];
}

// ====== Admin_IsAuthorized ======
stock bool:Admin_IsAuthorized(playerid, E_ADMIN_LEVEL:minLevel)
{
    if (!AccountSession_IsAuthenticated(playerid))
    {
        SendErrorMessage(playerid, "You are not logged into an account.");
        return false;
    }

    if (AdminSession_GetLevel(playerid) < minLevel)
    {
        PermissionError(playerid);
        return false;
    }

    return true;
}

// ====== Admin_IsDutyAuthorized ======
stock bool:Admin_IsDutyAuthorized(playerid, E_ADMIN_LEVEL:minLevel)
{
    if (!Admin_IsAuthorized(playerid, minLevel))
    {
        return false;
    }

    if (!AdminSession_IsOnDuty(playerid))
    {
        Admin_SendAdminText(playerid, "You must use /aduty before using this admin action.");
        return false;
    }

    return true;
}

// ====== Admin_SendSyntaxText ======
stock Admin_SendSyntaxText(playerid, const syntaxText[])
{
    new message[96];

    format(message, sizeof(message), "USAGE: "WHITE"%s", syntaxText);
    SendClientMessageEx(playerid, X11_GREY_80, message);
    return 1;
}

// ====== Admin_SendAdminText ======
stock Admin_SendAdminText(playerid, const text[])
{
    new message[144];

    format(message, sizeof(message), "ADMIN: "WHITE"%s", text);
    SendClientMessageEx(playerid, X11_TOMATO, message);
    return 1;
}

// ====== Admin_SendServerText ======
stock Admin_SendServerText(playerid, const text[])
{
    new message[144];

    format(message, sizeof(message), "SERVER: "WHITE"%s", text);
    SendClientMessageEx(playerid, X11_LIGHT_SKY_BLUE_1, message);
    return 1;
}

// ====== Admin_SendCommandList ======
stock Admin_SendCommandList(playerid)
{
    Admin_SendAdminText(playerid, "/adminlevel /ahelp /aduty");

    if (AdminSession_GetLevel(playerid) >= ADMIN_LEVEL_MODERATOR)
    {
        Admin_SendAdminText(playerid, "/spec /goto /gethere");
        Admin_SendAdminText(playerid, "Map marker teleport is available while on duty.");
    }

    if (AdminSession_GetLevel(playerid) >= ADMIN_LEVEL_HEAD_ADMIN)
    {
        Admin_SendAdminText(playerid, "/setadmin");
    }

    return 1;
}

// ====== Admin_ParsePlayerTarget ======
stock bool:Admin_ParsePlayerTarget(playerid, const params[], const syntaxText[], &targetid)
{
    if (!strlen(params))
    {
        Admin_SendSyntaxText(playerid, syntaxText);
        return false;
    }

    for (new i = 0; params[i] != '\0'; i++)
    {
        if (params[i] < '0' || params[i] > '9')
        {
            Admin_SendSyntaxText(playerid, syntaxText);
            return false;
        }
    }

    targetid = strval(params);

    if (targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid))
    {
        SendErrorMessage(playerid, "Invalid target player.");
        return false;
    }

    return true;
}

// ====== Admin_ParseSetAdminParams ======
stock bool:Admin_ParseSetAdminParams(playerid, const params[], &targetid, &level)
{
    new splitIndex;
    new targetText[16];
    new levelText[16];

    if (!strlen(params))
    {
        SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
        return false;
    }

    splitIndex = strfind(params, " ", true);

    if (splitIndex == -1)
    {
        SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
        return false;
    }

    strmid(targetText, params, 0, splitIndex, sizeof(targetText));
    strmid(levelText, params, splitIndex + 1, strlen(params), sizeof(levelText));

    if (!strlen(targetText) || !strlen(levelText))
    {
        SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
        return false;
    }

    for (new i = 0; targetText[i] != '\0'; i++)
    {
        if (targetText[i] < '0' || targetText[i] > '9')
        {
            SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
            return false;
        }
    }

    for (new i = 0; levelText[i] != '\0'; i++)
    {
        if (levelText[i] < '0' || levelText[i] > '9')
        {
            SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
            return false;
        }
    }

    targetid = strval(targetText);
    level = strval(levelText);

    return true;
}

// ====== Admin_OnGameModeInit ======
stock Admin_OnGameModeInit()
{
    return 1;
}

// ====== Admin_OnPlayerConnect ======
stock Admin_OnPlayerConnect(playerid)
{
    return AdminSession_Reset(playerid);
}

// ====== Admin_OnPlayerDisconnect ======
stock Admin_OnPlayerDisconnect(playerid, reason)
{
    #pragma unused reason
    return AdminSession_Reset(playerid);
}

// ====== Admin_OnPlayerClickMap ======
stock Admin_OnPlayerClickMap(playerid, Float:x, Float:y, Float:z)
{
    #pragma unused z

    if (!Admin_IsDutyAuthorized(playerid, ADMIN_LEVEL_HELPER))
    {
        return 0;
    }

    TogglePlayerSpectating(playerid, false);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPosFindZ(playerid, x, y, 999.0);
    Admin_SendAdminText(playerid, "Teleported to your map marker.");
    return 1;
}

// ====== Admin_LoadAccountLevel ======
stock Admin_LoadAccountLevel(playerid, accountId)
{
    new query[96];

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "SELECT `admin_level` FROM `accounts` WHERE `id` = %d LIMIT 1",
        accountId
    );
    mysql_tquery(Database_GetHandle(), query, "OnAdminAccountLevelLoaded", "dd", playerid, accountId);
    return 1;
}

// ====== Admin_SetAccountLevel ======
stock Admin_SetAccountLevel(accountId, level, assignedByAccountId)
{
    new query[128];
    #pragma unused assignedByAccountId

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "UPDATE `accounts` SET `admin_level` = %d, `updated_at` = NOW() WHERE `id` = %d LIMIT 1",
        level,
        accountId
    );
    mysql_tquery(Database_GetHandle(), query);
    return 1;
}

// ====== OnAdminAccountLevelLoaded ======
public OnAdminAccountLevelLoaded(playerid, accountId)
{
    new levelValue;

    if (cache_num_rows() > 0)
    {
        cache_get_value_name_int(0, "admin_level", levelValue);
    }

    if (AccountSession_IsAuthenticated(playerid) && AccountSession_GetId(playerid) == accountId)
    {
        AdminSession_SetLevel(playerid, E_ADMIN_LEVEL:levelValue);
    }

    return 1;
}

#include "modules/admin/command/command.inc"
