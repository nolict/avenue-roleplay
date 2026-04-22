#if defined AVE_CHARACTER_SELECT_INC
    #endinput
#endif
#define AVE_CHARACTER_SELECT_INC

// ====== Character_LoadSelection ======
stock Character_LoadSelection(playerid)
{
    new query[128];

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "SELECT `id`, `name` FROM `characters` WHERE `account_id` = %d ORDER BY `id` ASC LIMIT %d",
        AccountSession_GetId(playerid),
        MAX_CHARACTERS_PER_ACCOUNT
    );
    mysql_tquery(Database_GetHandle(), query, "OnCharacterSelectionLoaded", "d", playerid);
    return 1;
}

forward OnCharacterSelectionLoaded(playerid);
public OnCharacterSelectionLoaded(playerid)
{
    new rows = cache_num_rows();

    g_CharacterSelectionCount[playerid] = 0;

    for (new i = 0; i < rows && i < MAX_CHARACTERS_PER_ACCOUNT; i++)
    {
        g_CharacterSelectionIds[playerid][i] = 0;
        cache_get_value_name_int(i, "id", g_CharacterSelectionIds[playerid][i]);
        cache_get_value_name(i, "name", g_CharacterSelectionNames[playerid][i], MAX_CHARACTER_NAME + 1);
        g_CharacterSelectionCount[playerid]++;
    }

    if (g_CharacterSelectionCount[playerid] == 0)
    {
        SendServerMessage(playerid, "No character found. Create your first character.");
        return Character_ShowCreateNameDialog(playerid);
    }

    return Character_ShowSelectionDialog(playerid);
}
