#if defined AVE_CHARACTER_LOADER_INC
    #endinput
#endif
#define AVE_CHARACTER_LOADER_INC

#include <a_mysql>

#define MAX_CHARACTERS_PER_ACCOUNT (3)

enum E_CHARACTER_GENDER
{
    CHARACTER_GENDER_MALE = 0,
    CHARACTER_GENDER_FEMALE
}

new g_CharacterSelectionCount[MAX_PLAYERS];
new g_CharacterSelectionIds[MAX_PLAYERS][MAX_CHARACTERS_PER_ACCOUNT];
new g_CharacterSelectionNames[MAX_PLAYERS][MAX_CHARACTERS_PER_ACCOUNT][MAX_CHARACTER_NAME + 1];
new g_CharacterPendingName[MAX_PLAYERS][MAX_CHARACTER_NAME + 1];
new g_CharacterPendingGender[MAX_PLAYERS];

stock Character_EnsureSchema();
stock Character_LoadSelection(playerid);
stock Character_LoadAndSpawn(playerid, characterId);
stock Character_HandleCreateSubmit(playerid);
stock bool:Character_IsNameValid(const name[]);
stock Character_SavePosition(playerid);
stock Character_ShowCreateGenDialog(playerid);
forward dialog_CharacterSelect(playerid, response, listitem, inputtext[]);
forward dialog_CharacterCreateName(playerid, response, listitem, inputtext[]);
forward dialog_CharacterCreateGender(playerid, response, listitem, inputtext[]);

// ====== Character_OnGameModeInit ======
stock Character_OnGameModeInit()
{
    return Character_EnsureSchema();
}

// ====== Character_OnPlayerConnect ======
stock Character_OnPlayerConnect(playerid)
{
    Character_ResetPendingState(playerid);
    CharacterSession_Reset(playerid);
    return 1;
}

// ====== Character_OnPlayerDisconnect ======
stock Character_OnPlayerDisconnect(playerid, reason)
{
    #pragma unused reason
    Character_SavePosition(playerid);
    Character_ResetPendingState(playerid);
    CharacterSession_Reset(playerid);
    return 1;
}

// ====== Character_OnDialogResponse ======
stock Character_OnDialogResponse(playerid, dialogid, response, listitem, const inputtext[])
{
    #pragma unused playerid
    #pragma unused dialogid
    #pragma unused response
    #pragma unused listitem
    #pragma unused inputtext
    return 0;
}

// ====== Character_EnsureSchema ======
stock Character_EnsureSchema()
{
    mysql_tquery_file(Database_GetHandle(), "database/schema.sql");
    return 1;
}

// ====== Character_ShowGateway ======
stock Character_ShowGateway(playerid)
{
    if (!AccountSession_IsAuthenticated(playerid))
    {
        SendErrorMessage(playerid, "You are not logged into an account.");
        return Kick(playerid);
    }

    Character_ResetPendingState(playerid);
    return Character_LoadSelection(playerid);
}

// ====== Character_ResetPendingState ======
stock Character_ResetPendingState(playerid)
{
    g_CharacterSelectionCount[playerid] = 0;
    g_CharacterPendingName[playerid][0] = '\0';
    g_CharacterPendingGender[playerid] = _:CHARACTER_GENDER_MALE;

    for (new i = 0; i < MAX_CHARACTERS_PER_ACCOUNT; i++)
    {
        g_CharacterSelectionIds[playerid][i] = 0;
        g_CharacterSelectionNames[playerid][i][0] = '\0';
    }

    return 1;
}

// ====== Character_ShowSelectionDialog ======
stock Character_ShowSelectionDialog(playerid)
{
    new header[144];
    new dialogBody[512];
    new line[96];

    Interface_FormatDialogHeader(header, sizeof(header), "Character", "Character Selection");
    format(
        dialogBody,
        sizeof(dialogBody),
        ""TOMATO"Slot\tName\tStatus\n"
    );

    for (new i = 0; i < MAX_CHARACTERS_PER_ACCOUNT; i++)
    {
        if (i < g_CharacterSelectionCount[playerid])
        {
            format(
                line,
                sizeof(line),
                ""WHITE"%d\t%s\t"GREEN"READY\n",
                i + 1,
                g_CharacterSelectionNames[playerid][i]
            );
        }
        else
        {
            format(
                line,
                sizeof(line),
                ""WHITE"%d\t"GREY"-\t"YELLOW"CREATE\n",
                i + 1
            );
        }

        strcat(dialogBody, line, sizeof(dialogBody));
    }

    return Dialog_Show(playerid, CharacterSelect, DIALOG_STYLE_TABLIST_HEADERS, header,
        dialogBody,
        "Select",
        "Close");
}

// ====== Character_ShowCreateNameDialog ======
stock Character_ShowCreateNameDialog(playerid)
{
    new header[144];

    Interface_FormatDialogHeader(header, sizeof(header), "Character", "Create Character");
    return Dialog_Show(playerid, CharacterCreateName, DIALOG_STYLE_INPUT, header,
        ""TOMATO"Identity"WHITE"\n\
Enter a character name in the format First_Last.\n\n\
"YELLOW"Example"WHITE"\n\
John_Doe",
        "Next",
        "Back");
}

// ====== Character_ShowCreateGenDialog ======
stock Character_ShowCreateGenDialog(playerid)
{
    new header[144];

    Interface_FormatDialogHeader(header, sizeof(header), "Character", "Select Gender");
    return Dialog_Show(playerid, CharacterCreateGender, DIALOG_STYLE_LIST, header,
        "Male\nFemale",
        "Select",
        "Back");
}

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

Dialog:CharacterSelect(playerid, response, listitem, inputtext[])
{
    #pragma unused inputtext

    if (!response)
    {
        return Kick(playerid);
    }

    if (listitem < g_CharacterSelectionCount[playerid])
    {
        return Character_LoadAndSpawn(playerid, g_CharacterSelectionIds[playerid][listitem]);
    }

    if (g_CharacterSelectionCount[playerid] < MAX_CHARACTERS_PER_ACCOUNT && listitem == g_CharacterSelectionCount[playerid])
    {
        return Character_ShowCreateNameDialog(playerid);
    }

    return Character_ShowSelectionDialog(playerid);
}

Dialog:CharacterCreateName(playerid, response, listitem, inputtext[])
{
    #pragma unused listitem

    if (!response)
    {
        if (g_CharacterSelectionCount[playerid] > 0)
        {
            return Character_ShowSelectionDialog(playerid);
        }

        return Character_ShowCreateNameDialog(playerid);
    }

    if (!Character_IsNameValid(inputtext))
    {
        SendSyntaxMessage(playerid, "First_Last");
        return Character_ShowCreateNameDialog(playerid);
    }

    format(g_CharacterPendingName[playerid], MAX_CHARACTER_NAME + 1, "%s", inputtext);
    return Character_ShowCreateGenDialog(playerid);
}

Dialog:CharacterCreateGender(playerid, response, listitem, inputtext[])
{
    #pragma unused inputtext

    if (!response)
    {
        return Character_ShowCreateNameDialog(playerid);
    }

    if (listitem < _:CHARACTER_GENDER_MALE || listitem > _:CHARACTER_GENDER_FEMALE)
    {
        return Character_ShowCreateGenDialog(playerid);
    }

    g_CharacterPendingGender[playerid] = listitem;
    return Character_HandleCreateSubmit(playerid);
}

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

#define CHARACTER_DEFAULT_SPAWN_X (1702.0999)
#define CHARACTER_DEFAULT_SPAWN_Y (1607.5000)
#define CHARACTER_DEFAULT_SPAWN_Z (10.0200)
#define CHARACTER_DEFAULT_SPAWN_A (90.0000)
#define CHARACTER_DEFAULT_SPAWN_INTERIOR (0)
#define CHARACTER_DEFAULT_SPAWN_WORLD (0)

// ====== Character_GetDefaultSpawn ======
stock Character_GetDefaultSpawn(&Float:spawnX, &Float:spawnY, &Float:spawnZ, &Float:spawnA, &interior, &world)
{
    spawnX = CHARACTER_DEFAULT_SPAWN_X;
    spawnY = CHARACTER_DEFAULT_SPAWN_Y;
    spawnZ = CHARACTER_DEFAULT_SPAWN_Z;
    spawnA = CHARACTER_DEFAULT_SPAWN_A;
    interior = CHARACTER_DEFAULT_SPAWN_INTERIOR;
    world = CHARACTER_DEFAULT_SPAWN_WORLD;
    return 1;
}

// ====== Character_SavePosition ======
stock Character_SavePosition(playerid)
{
    new query[256];
    new Float:posX;
    new Float:posY;
    new Float:posZ;
    new Float:posA;
    new interior;
    new world;

    if (!CharacterSession_IsActive(playerid) || !Database_IsConnected())
    {
        return 1;
    }

    GetPlayerPos(playerid, posX, posY, posZ);
    GetPlayerFacingAngle(playerid, posA);
    interior = GetPlayerInterior(playerid);
    world = GetPlayerVirtualWorld(playerid);

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "UPDATE `characters` SET `pos_x` = %f, `pos_y` = %f, `pos_z` = %f, `pos_a` = %f, `interior` = %d, `world` = %d, `updated_at` = NOW() WHERE `id` = %d LIMIT 1",
        posX,
        posY,
        posZ,
        posA,
        interior,
        world,
        CharacterSession_GetId(playerid)
    );
    mysql_tquery(Database_GetHandle(), query);
    return 1;
}

// ====== Character_LoadAndSpawn ======
stock Character_LoadAndSpawn(playerid, characterId)
{
    new query[196];

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "SELECT `id`, `name`, `skin`, `pos_x`, `pos_y`, `pos_z`, `pos_a`, `interior`, `world` FROM `characters` WHERE `id` = %d AND `account_id` = %d LIMIT 1",
        characterId,
        AccountSession_GetId(playerid)
    );
    mysql_tquery(Database_GetHandle(), query, "OnCharacterLoadAndSpawn", "dd", playerid, characterId);
    return 1;
}

forward OnCharacterLoadAndSpawn(playerid, characterId);
public OnCharacterLoadAndSpawn(playerid, characterId)
{
    new loadedCharacterId;
    new characterName[MAX_CHARACTER_NAME + 1];
    new skin;
    new Float:spawnX;
    new Float:spawnY;
    new Float:spawnZ;
    new Float:spawnA;
    new interior;
    new world;

    #pragma unused characterId

    if (cache_num_rows() == 0)
    {
        SendErrorMessage(playerid, "Character not found.");
        return Character_ShowGateway(playerid);
    }

    cache_get_value_name_int(0, "id", loadedCharacterId);
    cache_get_value_name(0, "name", characterName, MAX_CHARACTER_NAME + 1);
    cache_get_value_name_int(0, "skin", skin);
    cache_get_value_name_float(0, "pos_x", spawnX);
    cache_get_value_name_float(0, "pos_y", spawnY);
    cache_get_value_name_float(0, "pos_z", spawnZ);
    cache_get_value_name_float(0, "pos_a", spawnA);
    cache_get_value_name_int(0, "interior", interior);
    cache_get_value_name_int(0, "world", world);

    CharacterSession_SetActive(playerid, loadedCharacterId, characterName);

    SetPlayerInterior(playerid, interior);
    SetPlayerVirtualWorld(playerid, world);
    SetSpawnInfo(
        playerid,
        NO_TEAM,
        skin,
        spawnX,
        spawnY,
        spawnZ,
        spawnA,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0
    );
    SpawnPlayer(playerid);
    SetCameraBehindPlayer(playerid);

    return 1;
}
