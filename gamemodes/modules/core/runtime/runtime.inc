#if defined _avenue_core_runtime_included
    #endinput
#endif
#define _avenue_core_runtime_included

#include <modules/interface/messages.inc>
#include <modules/interface/dialog/shared/header.inc>
#include <modules/interface/textdraw/server/loading.inc>

#include <modules/core/config/database.inc>
#include <modules/core/database/connection.inc>

#include <modules/player/loading.inc>
#include <modules/player/account/session/state.inc>
#include <modules/player/character/session/state.inc>
#include <modules/admin/admin.inc>
#include <modules/player/character.inc>
#include <modules/player/account.inc>

// ====== Runtime_OnGameModeInit ======
stock Runtime_OnGameModeInit()
{
    SetGameModeText("Avenue Roleplay");
    UsePlayerPedAnims();
    DisableInteriorEnterExits();

    if (!Database_Connect())
    {
        return 0;
    }

    Admin_OnGameModeInit();
    Character_OnGameModeInit();
    Account_OnGameModeInit();
    return 1;
}

// ====== Runtime_OnGameModeExit ======
stock Runtime_OnGameModeExit()
{
    Database_Disconnect();
    return 1;
}

// ====== Runtime_OnPlayerConnect ======
stock Runtime_OnPlayerConnect(playerid)
{
    PlayerLoading_OnPlayerConnect(playerid);
    Admin_OnPlayerConnect(playerid);
    Character_OnPlayerConnect(playerid);
    return Account_OnPlayerConnect(playerid);
}

// ====== Runtime_OnPlayerDisconnect ======
stock Runtime_OnPlayerDisconnect(playerid, reason)
{
    PlayerLoading_OnDisconnect(playerid);
    Admin_OnPlayerDisconnect(playerid, reason);
    Character_OnPlayerDisconnect(playerid, reason);
    return Account_OnPlayerDisconnect(playerid, reason);
}

// ====== Runtime_OnDialogResponse ======
stock Runtime_OnDialogResponse(playerid, dialogid, response, listitem, const inputtext[])
{
    if (Account_OnDialogResponse(playerid, dialogid, response, listitem, inputtext))
    {
        return 1;
    }

    return Character_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
}

// ====== Runtime_OnPlayerClickMap ======
stock Runtime_OnPlayerClickMap(playerid, Float:x, Float:y, Float:z)
{
    return Admin_OnPlayerClickMap(playerid, x, y, z);
}
