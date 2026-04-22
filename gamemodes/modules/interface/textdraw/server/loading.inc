#if defined AVE_UI_TXD_SRV_LOADING_INC
    #endinput
#endif
#define AVE_UI_TXD_SRV_LOADING_INC

new PlayerText:g_InterfaceLoadingTitle[MAX_PLAYERS];
new PlayerText:g_InterfaceLoadingSubtitle[MAX_PLAYERS];
new bool:g_InterfaceLoadingVisible[MAX_PLAYERS];

// ====== InterfaceLoading_Reset ======
stock InterfaceLoading_Reset(playerid)
{
    g_InterfaceLoadingVisible[playerid] = false;
    return 1;
}

// ====== InterfaceLoading_Show ======
stock InterfaceLoading_Show(playerid, const titleText[], const subtitleText[])
{
    if (g_InterfaceLoadingVisible[playerid])
    {
        InterfaceLoading_Hide(playerid);
    }

    g_InterfaceLoadingTitle[playerid] = CreatePlayerTextDraw(playerid, 320.0, 188.0, titleText);
    PlayerTextDrawAlignment(playerid, g_InterfaceLoadingTitle[playerid], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawBackgroundColour(playerid, g_InterfaceLoadingTitle[playerid], 255);
    PlayerTextDrawFont(playerid, g_InterfaceLoadingTitle[playerid], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, g_InterfaceLoadingTitle[playerid], 0.34, 1.8);
    PlayerTextDrawColour(playerid, g_InterfaceLoadingTitle[playerid], X11_LIGHT_SKY_BLUE_1);
    PlayerTextDrawSetOutline(playerid, g_InterfaceLoadingTitle[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_InterfaceLoadingTitle[playerid], true);

    g_InterfaceLoadingSubtitle[playerid] = CreatePlayerTextDraw(playerid, 320.0, 208.0, subtitleText);
    PlayerTextDrawAlignment(playerid, g_InterfaceLoadingSubtitle[playerid], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawBackgroundColour(playerid, g_InterfaceLoadingSubtitle[playerid], 255);
    PlayerTextDrawFont(playerid, g_InterfaceLoadingSubtitle[playerid], TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, g_InterfaceLoadingSubtitle[playerid], 0.2, 1.1);
    PlayerTextDrawColour(playerid, g_InterfaceLoadingSubtitle[playerid], X11_GREY_80);
    PlayerTextDrawSetOutline(playerid, g_InterfaceLoadingSubtitle[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_InterfaceLoadingSubtitle[playerid], true);

    PlayerTextDrawShow(playerid, g_InterfaceLoadingTitle[playerid]);
    PlayerTextDrawShow(playerid, g_InterfaceLoadingSubtitle[playerid]);
    g_InterfaceLoadingVisible[playerid] = true;
    return 1;
}

// ====== InterfaceLoading_Hide ======
stock InterfaceLoading_Hide(playerid)
{
    if (!g_InterfaceLoadingVisible[playerid])
    {
        return 1;
    }

    PlayerTextDrawHide(playerid, g_InterfaceLoadingTitle[playerid]);
    PlayerTextDrawHide(playerid, g_InterfaceLoadingSubtitle[playerid]);
    PlayerTextDrawDestroy(playerid, g_InterfaceLoadingTitle[playerid]);
    PlayerTextDrawDestroy(playerid, g_InterfaceLoadingSubtitle[playerid]);
    g_InterfaceLoadingVisible[playerid] = false;
    return 1;
}
