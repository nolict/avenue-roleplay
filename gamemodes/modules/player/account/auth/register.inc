#if defined AVE_ACC_AUTH_REG_INC
    #endinput
#endif
#define AVE_ACC_AUTH_REG_INC

// ====== Account_EnsureSchema ======
stock Account_EnsureSchema()
{
    mysql_tquery_file(Database_GetHandle(), "database/schema.sql");
    return 1;
}

// ====== Account_HandleRegisterPasswordSubmit ======
stock Acc_HandleRegPassSubmit(playerid, const inputtext[])
{
    if (!strlen(g_AccountPendingUsername[playerid]))
    {
        SendErrorMessage(playerid, "Register username has not been set.");
        return Acc_KickEx(playerid);
    }

    bcrypt_hash(playerid, "OnAccountPasswordHashed", inputtext, ACCOUNT_BCRYPT_COST);
    return 1;
}

forward OnAccountPasswordHashed(playerid);
public OnAccountPasswordHashed(playerid)
{
    new hash[MAX_BCRYPT_HASH];
    new query[320];

    bcrypt_get_hash(hash, sizeof(hash));

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "INSERT INTO `accounts` (`username`, `password_hash`, `created_at`, `updated_at`) VALUES ('%e', '%e', NOW(), NOW())",
        g_AccountPendingUsername[playerid],
        hash
    );
    mysql_tquery(Database_GetHandle(), query, "OnAccountRegisterInsert", "d", playerid);
    return 1;
}

forward OnAccountRegisterInsert(playerid);
public OnAccountRegisterInsert(playerid)
{
    new accountId = cache_insert_id();

    if (accountId <= 0)
    {
        SendErrorMessage(playerid, "Failed to create account.");
        Acc_ResetPendingState(playerid);
        return Acc_KickEx(playerid);
    }

    AccountSession_SetAuthenticated(playerid, accountId, g_AccountPendingUsername[playerid]);
    Acc_ResetPendingState(playerid);
    SendServerMessage(playerid, "UCP registration successful.");
    return Character_ShowGateway(playerid);
}
