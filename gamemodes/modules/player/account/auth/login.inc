#if defined AVE_ACC_AUTH_LOGIN_INC
    #endinput
#endif
#define AVE_ACC_AUTH_LOGIN_INC

// ====== Account_HandleLoginPasswordSubmit ======
stock Acc_HandleLoginPassSubmit(playerid, const inputtext[])
{
    if (g_AccountPendingId[playerid] <= 0 || !strlen(g_AccountPendingHash[playerid]))
    {
        SendErrorMessage(playerid, "Login data is incomplete.");
        return Acc_KickEx(playerid);
    }

    bcrypt_verify(playerid, "OnAccountPasswordVerified", inputtext, g_AccountPendingHash[playerid]);
    return 1;
}

// ====== Account_UpdateLastLoginAt ======
stock Account_UpdateLastLoginAt(accountId)
{
    new query[96];

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "UPDATE `accounts` SET `last_login_at` = NOW(), `updated_at` = NOW() WHERE `id` = %d",
        accountId
    );
    mysql_tquery(Database_GetHandle(), query);
    return 1;
}

forward OnAccountPasswordVerified(playerid, bool:success);
public OnAccountPasswordVerified(playerid, bool:success)
{
    if (!success)
    {
        SendErrorMessage(playerid, "Incorrect password.");
        return Acc_ShowLoginPassDialog(playerid);
    }

    AccountSession_SetAuthenticated(playerid, g_AccountPendingId[playerid], g_AccountPendingUsername[playerid]);
    Account_UpdateLastLoginAt(g_AccountPendingId[playerid]);
    Acc_ResetPendingState(playerid);
    SendServerMessage(playerid, "UCP login successful.");
    return Character_ShowGateway(playerid);
}
