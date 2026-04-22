#if defined AVE_ACC_SESSION_INC
    #endinput
#endif
#define AVE_ACC_SESSION_INC

#if !defined MAX_ACCOUNT_USERNAME
    #define MAX_ACCOUNT_USERNAME (24)
#endif

enum E_ACCOUNT_SESSION_DATA
{
    bool:ACCOUNT_SESSION_AUTHENTICATED,
    ACCOUNT_SESSION_ID,
    ACCOUNT_SESSION_USERNAME[MAX_ACCOUNT_USERNAME + 1]
}

new g_AccountSession[MAX_PLAYERS][E_ACCOUNT_SESSION_DATA];

// ====== AccountSession_Reset ======
stock AccountSession_Reset(playerid)
{
    g_AccountSession[playerid][ACCOUNT_SESSION_AUTHENTICATED] = false;
    g_AccountSession[playerid][ACCOUNT_SESSION_ID] = 0;
    g_AccountSession[playerid][ACCOUNT_SESSION_USERNAME][0] = '\0';
    return 1;
}

// ====== AccountSession_SetAuthenticated ======
stock AccountSession_SetAuthenticated(playerid, accountId, const username[])
{
    g_AccountSession[playerid][ACCOUNT_SESSION_AUTHENTICATED] = true;
    g_AccountSession[playerid][ACCOUNT_SESSION_ID] = accountId;
    format(
        g_AccountSession[playerid][ACCOUNT_SESSION_USERNAME],
        MAX_ACCOUNT_USERNAME + 1,
        "%s",
        username
    );
    return 1;
}

// ====== AccountSession_IsAuthenticated ======
stock bool:AccountSession_IsAuthenticated(playerid)
{
    return g_AccountSession[playerid][ACCOUNT_SESSION_AUTHENTICATED];
}

// ====== AccountSession_GetId ======
stock AccountSession_GetId(playerid)
{
    return g_AccountSession[playerid][ACCOUNT_SESSION_ID];
}
