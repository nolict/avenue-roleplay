#if defined AVE_PLAYER_LOADING_INC
    #endinput
#endif
#define AVE_PLAYER_LOADING_INC

#define PLAYER_LOADING_RELEASE_DELAY (750)

new bool:g_PlayerLoadingActive[MAX_PLAYERS];
new g_PlayerLoadingToken[MAX_PLAYERS];

forward OnPlayerLoadingRelease(playerid, token);

// ====== PlayerLoading_OnPlayerConnect ======
stock PlayerLoading_OnPlayerConnect(playerid)
{
    g_PlayerLoadingActive[playerid] = false;
    g_PlayerLoadingToken[playerid] = 0;
    InterfaceLoading_Reset(playerid);
    TogglePlayerControllable(playerid, true);
    return 1;
}

// ====== PlayerLoading_OnDisconnect ======
stock PlayerLoading_OnDisconnect(playerid)
{
    g_PlayerLoadingActive[playerid] = false;
    g_PlayerLoadingToken[playerid] = 0;
    InterfaceLoading_Hide(playerid);
    TogglePlayerControllable(playerid, true);
    return 1;
}

// ====== PlayerLoading_Begin ======
stock PlayerLoading_Begin(playerid, const titleText[], const subtitleText[] = "Please wait")
{
    g_PlayerLoadingToken[playerid]++;
    g_PlayerLoadingActive[playerid] = true;
    TogglePlayerControllable(playerid, false);
    InterfaceLoading_Show(playerid, titleText, subtitleText);
    return g_PlayerLoadingToken[playerid];
}

// ====== PlayerLoading_End ======
stock PlayerLoading_End(playerid)
{
    g_PlayerLoadingActive[playerid] = false;
    InterfaceLoading_Hide(playerid);
    TogglePlayerControllable(playerid, true);
    return 1;
}

// ====== PlayerLoading_ScheduleEnd ======
stock PlayerLoading_ScheduleEnd(playerid, delay = PLAYER_LOADING_RELEASE_DELAY)
{
    if (!g_PlayerLoadingActive[playerid])
    {
        return 1;
    }

    SetTimerEx("OnPlayerLoadingRelease", delay, false, "dd", playerid, g_PlayerLoadingToken[playerid]);
    return 1;
}

// ====== PlayerLoading_IsActive ======
stock bool:PlayerLoading_IsActive(playerid)
{
    return g_PlayerLoadingActive[playerid];
}

public OnPlayerLoadingRelease(playerid, token)
{
    if (!IsPlayerConnected(playerid))
    {
        return 0;
    }

    if (!g_PlayerLoadingActive[playerid] || g_PlayerLoadingToken[playerid] != token)
    {
        return 0;
    }

    return PlayerLoading_End(playerid);
}
