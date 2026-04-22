#if defined AVE_CHARACTER_SPAWN_INC
    #endinput
#endif
#define AVE_CHARACTER_SPAWN_INC

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
