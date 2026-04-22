#include <open.mp>

#include "modules/core/runtime/runtime.inc"

main()
{
}

// ====== OnGameModeInit ======
public OnGameModeInit()
{
    return Runtime_OnGameModeInit();
}

// ====== OnGameModeExit ======
public OnGameModeExit()
{
    return Runtime_OnGameModeExit();
}

// ====== OnPlayerConnect ======
public OnPlayerConnect(playerid)
{
    return Runtime_OnPlayerConnect(playerid);
}

// ====== OnPlayerDisconnect ======
public OnPlayerDisconnect(playerid, reason)
{
    return Runtime_OnPlayerDisconnect(playerid, reason);
}

// ====== OnDialogResponse ======
public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    return Runtime_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
}

// ====== OnPlayerClickMap ======
public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    return Runtime_OnPlayerClickMap(playerid, fX, fY, fZ);
}
