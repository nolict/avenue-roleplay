#if defined _avenue_account_loader_included
    #endinput
#endif
#define _avenue_account_loader_included

#include <a_mysql>
#include <samp_bcrypt>

#define MAX_ACCOUNT_USERNAME (24)
#define MIN_ACCOUNT_PASSWORD (6)
#define MAX_ACCOUNT_PASSWORD (72)
#define MAX_BCRYPT_HASH (128)

enum E_ACCOUNT_PENDING_ACTION
{
    ACCOUNT_PENDING_NONE = 0,
    ACCOUNT_PENDING_REGISTER,
    ACCOUNT_PENDING_LOGIN
}

new g_AccountPendingUsername[MAX_PLAYERS][MAX_ACCOUNT_USERNAME + 1];
new g_AccountPendingHash[MAX_PLAYERS][MAX_BCRYPT_HASH];
new g_AccountPendingId[MAX_PLAYERS];
new g_AccountPendingAction[MAX_PLAYERS];

stock bool:Acc_IsUsernameValid(const username[]);
stock Account_EnsureSchema();
stock Acc_HandleRegPassSubmit(playerid, const inputtext[]);
stock Acc_HandleLoginPassSubmit(playerid, const inputtext[]);
stock Character_ShowGateway(playerid);
forward dialog_AccountInvalidName(playerid, response, listitem, inputtext[]);
forward dialog_AccountRegisterPassword(playerid, response, listitem, inputtext[]);
forward dialog_AccountLoginPassword(playerid, response, listitem, inputtext[]);

// ====== Account_OnGameModeInit ======
stock Account_OnGameModeInit()
{
    return Account_EnsureSchema();
}

// ====== Account_OnPlayerConnect ======
stock Account_OnPlayerConnect(playerid)
{
    new playerName[MAX_PLAYER_NAME + 1];

    Acc_ResetPendingState(playerid);
    AccountSession_Reset(playerid);

    GetPlayerName(playerid, playerName, sizeof(playerName));

    if (!Acc_IsUsernameValid(playerName))
    {
        return Acc_ShowInvalidNameDialog(playerid);
    }

    format(g_AccountPendingUsername[playerid], MAX_ACCOUNT_USERNAME + 1, "%s", playerName);
    return Acc_BeginAutoAuth(playerid);
}

// ====== Account_OnPlayerDisconnect ======
stock Account_OnPlayerDisconnect(playerid, reason)
{
    #pragma unused reason
    Acc_ResetPendingState(playerid);
    AccountSession_Reset(playerid);
    return 1;
}

// ====== Account_IsUsernameValid ======
stock bool:Acc_IsUsernameValid(const username[])
{
    new usernameLength = strlen(username);

    if (usernameLength < 3 || usernameLength > MAX_ACCOUNT_USERNAME)
    {
        return false;
    }

    for (new i = 0; i < usernameLength; i++)
    {
        if (!((username[i] >= '0' && username[i] <= '9')
            || (username[i] >= 'A' && username[i] <= 'Z')
            || (username[i] >= 'a' && username[i] <= 'z')))
        {
            return false;
        }
    }

    return true;
}

// ====== Account_IsPasswordValid ======
stock bool:Acc_IsPasswordValid(const password[])
{
    new passwordLength = strlen(password);
    return passwordLength >= MIN_ACCOUNT_PASSWORD && passwordLength <= MAX_ACCOUNT_PASSWORD;
}

// ====== Account_OnDialogResponse ======
stock Account_OnDialogResponse(playerid, dialogid, response, listitem, const inputtext[])
{
    #pragma unused playerid
    #pragma unused dialogid
    #pragma unused response
    #pragma unused listitem
    #pragma unused inputtext

    return 0;
}

// ====== Account_BeginAutoAuth ======
stock Acc_BeginAutoAuth(playerid)
{
    new query[196];

    if (!Database_IsConnected())
    {
        SendErrorMessage(playerid, "Database belum siap. Coba lagi nanti.");
        return Acc_KickEx(playerid);
    }

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "SELECT `id`, `password_hash` FROM `accounts` WHERE `username` = '%e' LIMIT 1",
        g_AccountPendingUsername[playerid]
    );
    mysql_tquery(Database_GetHandle(), query, "OnAccountAutoAuthLookup", "d", playerid);
    return 1;
}

// ====== Account_ShowInvalidNameDialog ======
stock Acc_ShowInvalidNameDialog(playerid)
{
    new header[144];

    Interface_FormatDialogHeader(header, sizeof(header), "Account", "Invalid Nickname");
    return Dialog_Show(playerid, AccountInvalidName, DIALOG_STYLE_MSGBOX, header,
        ""TOMATO"Issue"WHITE"\n\
Your nickname may only contain letters and numbers.\n\n\
"YELLOW"Action"WHITE"\n\
Change your nickname and reconnect to the server.",
        "Close",
        "");
}

// ====== Account_ShowRegisterPasswordDialog ======
stock Acc_ShowRegPassDialog(playerid)
{
    new header[144];

    Interface_FormatDialogHeader(header, sizeof(header), "Account", "Register Password");
    return Dialog_Show(playerid, AccountRegisterPassword, DIALOG_STYLE_PASSWORD, header,
        ""TOMATO"UCP Setup"WHITE"\n\
Your account username is taken from your current nickname.\n\n\
"YELLOW"Password Rules"WHITE"\n\
- Minimum 6 characters\n\
- Maximum 72 characters",
        "Register",
        "Close");
}

// ====== Account_ShowLoginPasswordDialog ======
stock Acc_ShowLoginPassDialog(playerid)
{
    new header[144];

    Interface_FormatDialogHeader(header, sizeof(header), "Account", "Login Password");
    return Dialog_Show(playerid, AccountLoginPassword, DIALOG_STYLE_PASSWORD, header,
        ""TOMATO"UCP Authentication"WHITE"\n\
Your account username is taken from your current nickname.\n\n\
"YELLOW"Instructions"WHITE"\n\
Enter your account password to continue.",
        "Login",
        "Close");
}

Dialog:AccountInvalidName(playerid, response, listitem, inputtext[])
{
    #pragma unused response
    #pragma unused listitem
    #pragma unused inputtext
    return Acc_KickEx(playerid);
}

Dialog:AccountRegisterPassword(playerid, response, listitem, inputtext[])
{
    #pragma unused listitem

    if (!response)
    {
        return Acc_KickEx(playerid);
    }

    if (!Acc_IsPasswordValid(inputtext))
    {
        SendErrorMessage(playerid, "Password must be 6-72 characters.");
        return Acc_ShowRegPassDialog(playerid);
    }

    return Acc_HandleRegPassSubmit(playerid, inputtext);
}

Dialog:AccountLoginPassword(playerid, response, listitem, inputtext[])
{
    #pragma unused listitem

    if (!response)
    {
        return Acc_KickEx(playerid);
    }

    if (!Acc_IsPasswordValid(inputtext))
    {
        SendErrorMessage(playerid, "Password must be 6-72 characters.");
        return Acc_ShowLoginPassDialog(playerid);
    }

    return Acc_HandleLoginPassSubmit(playerid, inputtext);
}

// ====== Account_KickEx ======
stock Acc_KickEx(playerid)
{
    SetTimerEx("OnAccountDelayedKick", 250, false, "d", playerid);
    return 1;
}

// ====== Account_ResetPendingState ======
stock Acc_ResetPendingState(playerid)
{
    g_AccountPendingUsername[playerid][0] = '\0';
    g_AccountPendingHash[playerid][0] = '\0';
    g_AccountPendingId[playerid] = 0;
    g_AccountPendingAction[playerid] = _:ACCOUNT_PENDING_NONE;
    return 1;
}

forward OnAccountAutoAuthLookup(playerid);
public OnAccountAutoAuthLookup(playerid)
{
    if (cache_num_rows() == 0)
    {
        g_AccountPendingAction[playerid] = _:ACCOUNT_PENDING_REGISTER;
        return Acc_ShowRegPassDialog(playerid);
    }

    g_AccountPendingAction[playerid] = _:ACCOUNT_PENDING_LOGIN;
    cache_get_value_name_int(0, "id", g_AccountPendingId[playerid]);
    cache_get_value_name(0, "password_hash", g_AccountPendingHash[playerid], MAX_BCRYPT_HASH);
    return Acc_ShowLoginPassDialog(playerid);
}

forward OnAccountDelayedKick(playerid);
public OnAccountDelayedKick(playerid)
{
    return Kick(playerid);
}

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
