#if defined AVE_CHARACTER_CREATE_INC
    #endinput
#endif
#define AVE_CHARACTER_CREATE_INC

// ====== Character_IsUppercaseLetter ======
stock bool:Character_IsUppercaseLetter(character)
{
    return character >= 'A' && character <= 'Z';
}

// ====== Character_IsLowercaseLetter ======
stock bool:Character_IsLowercaseLetter(character)
{
    return character >= 'a' && character <= 'z';
}

// ====== Character_IsNamePartValid ======
stock bool:Character_IsNamePartValid(const name[], startIndex, endIndex)
{
    if (endIndex - startIndex < 2)
    {
        return false;
    }

    if (!Character_IsUppercaseLetter(name[startIndex]))
    {
        return false;
    }

    for (new i = startIndex + 1; i < endIndex; i++)
    {
        if (!Character_IsLowercaseLetter(name[i]))
        {
            return false;
        }
    }

    return true;
}

// ====== Character_IsNameValid ======
stock bool:Character_IsNameValid(const name[])
{
    new length = strlen(name);
    new underscoreIndex = strfind(name, "_");

    if (length < 5 || length > MAX_CHARACTER_NAME)
    {
        return false;
    }

    if (underscoreIndex <= 0 || underscoreIndex >= length - 1)
    {
        return false;
    }

    if (strfind(name, "_", true, underscoreIndex + 1) != -1)
    {
        return false;
    }

    return Character_IsNamePartValid(name, 0, underscoreIndex)
        && Character_IsNamePartValid(name, underscoreIndex + 1, length);
}

// ====== Character_GetDefaultSkin ======
stock Character_GetDefaultSkin(gender)
{
    return (gender == _:CHARACTER_GENDER_FEMALE) ? 56 : 78;
}

// ====== Character_HandleCreateSubmit ======
stock Character_HandleCreateSubmit(playerid)
{
    new query[160];

    if (!AccountSession_IsAuthenticated(playerid))
    {
        SendErrorMessage(playerid, "You are not logged into an account.");
        return Kick(playerid);
    }

    if (g_CharacterSelectionCount[playerid] >= MAX_CHARACTERS_PER_ACCOUNT)
    {
        SendErrorMessage(playerid, "Character slots are full.");
        return Character_ShowSelectionDialog(playerid);
    }

    if (!Character_IsNameValid(g_CharacterPendingName[playerid]))
    {
        SendErrorMessage(playerid, "Character name is invalid.");
        return Character_ShowCreateNameDialog(playerid);
    }

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "SELECT `id` FROM `characters` WHERE `name` = '%e' LIMIT 1",
        g_CharacterPendingName[playerid]
    );
    mysql_tquery(Database_GetHandle(), query, "OnCharacterCreateNameCheck", "d", playerid);
    return 1;
}

forward OnCharacterCreateNameCheck(playerid);
public OnCharacterCreateNameCheck(playerid)
{
    new query[320];
    new Float:spawnX;
    new Float:spawnY;
    new Float:spawnZ;
    new Float:spawnA;
    new interior;
    new world;

    if (cache_num_rows() > 0)
    {
        SendErrorMessage(playerid, "Character name is already in use.");
        return Character_ShowCreateNameDialog(playerid);
    }

    Character_GetDefaultSpawn(spawnX, spawnY, spawnZ, spawnA, interior, world);

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "INSERT INTO `characters` (`account_id`, `name`, `gender`, `skin`, `pos_x`, `pos_y`, `pos_z`, `pos_a`, `interior`, `world`, `created_at`, `updated_at`) VALUES (%d, '%e', %d, %d, %f, %f, %f, %f, %d, %d, NOW(), NOW())",
        AccountSession_GetId(playerid),
        g_CharacterPendingName[playerid],
        g_CharacterPendingGender[playerid],
        Character_GetDefaultSkin(g_CharacterPendingGender[playerid]),
        spawnX,
        spawnY,
        spawnZ,
        spawnA,
        interior,
        world
    );
    mysql_tquery(Database_GetHandle(), query, "OnCharacterCreateInsert", "d", playerid);
    return 1;
}

forward OnCharacterCreateInsert(playerid);
public OnCharacterCreateInsert(playerid)
{
    new characterId = cache_insert_id();

    if (characterId <= 0)
    {
        SendErrorMessage(playerid, "Failed to create character.");
        return Character_ShowCreateNameDialog(playerid);
    }

    SendServerMessage(playerid, "Character created successfully.");
    return Character_LoadAndSpawn(playerid, characterId);
}
