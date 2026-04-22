#if defined AVE_CHARACTER_SESSION_INC
    #endinput
#endif
#define AVE_CHARACTER_SESSION_INC

#define MAX_CHARACTER_NAME (24)

enum E_CHARACTER_SESSION_DATA
{
    bool:CHARACTER_SESSION_ACTIVE,
    CHARACTER_SESSION_ID,
    CHARACTER_SESSION_NAME[MAX_CHARACTER_NAME + 1]
}

new g_CharacterSession[MAX_PLAYERS][E_CHARACTER_SESSION_DATA];

// ====== CharacterSession_Reset ======
stock CharacterSession_Reset(playerid)
{
    g_CharacterSession[playerid][CHARACTER_SESSION_ACTIVE] = false;
    g_CharacterSession[playerid][CHARACTER_SESSION_ID] = 0;
    g_CharacterSession[playerid][CHARACTER_SESSION_NAME][0] = '\0';
    return 1;
}

// ====== CharacterSession_SetActive ======
stock CharacterSession_SetActive(playerid, characterId, const characterName[])
{
    g_CharacterSession[playerid][CHARACTER_SESSION_ACTIVE] = true;
    g_CharacterSession[playerid][CHARACTER_SESSION_ID] = characterId;
    format(
        g_CharacterSession[playerid][CHARACTER_SESSION_NAME],
        MAX_CHARACTER_NAME + 1,
        "%s",
        characterName
    );
    return 1;
}

// ====== CharacterSession_IsActive ======
stock bool:CharacterSession_IsActive(playerid)
{
    return g_CharacterSession[playerid][CHARACTER_SESSION_ACTIVE];
}

// ====== CharacterSession_GetId ======
stock CharacterSession_GetId(playerid)
{
    return g_CharacterSession[playerid][CHARACTER_SESSION_ID];
}
