#if defined AVE_ADMIN_LOADER_INC
    #endinput
#endif
#define AVE_ADMIN_LOADER_INC

#include <a_mysql>

#include "modules/admin/permission/level.inc"

forward OnAdminAccountLevelLoaded(playerid, accountId);

enum E_ADMIN_INTERIOR_DATA
{
    E_ADMIN_INTERIOR_COMMAND_ID,
    E_ADMIN_INTERIOR_WORLD_ID,
    Float:E_ADMIN_INTERIOR_X,
    Float:E_ADMIN_INTERIOR_Y,
    Float:E_ADMIN_INTERIOR_Z,
    Float:E_ADMIN_INTERIOR_A,
    E_ADMIN_INTERIOR_NAME[48]
}

static const g_AdminInteriorData[][E_ADMIN_INTERIOR_DATA] =
{
    {1, 5, 770.8033, -0.7033, 1000.7267, 22.8599, "Ganton Gym"},
    {2, 3, 974.0177, -9.5937, 1001.1484, 22.6045, "Brothel"},
    {3, 3, 961.9308, -51.9071, 1001.1172, 95.5381, "Brothel2"},
    {4, 3, 830.6016, 5.9404, 1004.1797, 125.8149, "Inside Track Betting"},
    {5, 3, 1037.8276, 0.3970, 1001.2845, 353.9335, "Blastin' Fools Records"},
    {6, 3, 1212.1489, -28.5388, 1000.9531, 170.5692, "The Big Spread Ranch"},
    {7, 18, 1290.4106, 1.9512, 1001.0201, 179.9419, "Warehouse 1"},
    {8, 1, 1412.1472, -2.2836, 1000.9241, 114.6610, "Warehouse 2"},
    {9, 9, 1527.0468, -12.0236, 1002.0971, 350.0013, "B Dup's Apartment"},
    {10, 0, 2547.1853, 2824.2493, 10.8203, 262.7038, "B Dup's Crack Palace"},
    {11, 3, 612.2191, -123.9028, 997.9922, 266.5704, "Wheel Arch Angels"},
    {12, 3, 512.9291, -11.6929, 1001.5653, 198.7669, "OG Loc's House"},
    {13, 3, 418.4666, -80.4595, 1001.8047, 343.2358, "Barber Shop"},
    {14, 3, 386.5259, 173.6381, 1008.3828, 63.7399, "Planning Department"},
    {15, 3, 288.4723, 170.0647, 1007.1794, 22.0477, "Las Venturas Police Department"},
    {16, 3, 206.4627, -137.7076, 1003.0938, 10.9347, "Pro-Laps"},
    {17, 3, -100.2674, -22.9376, 1000.7188, 17.2850, "Sex Shop"},
    {18, 3, -201.2236, -43.2465, 1002.2734, 45.8613, "Las Venturas Tattoo Parlor"},
    {19, 17, -202.9381, -6.7006, 1002.2734, 204.2693, "Lost San Fierro Tattoo Parlor"},
    {20, 17, -17.9142, -173.4321, 1003.5469, 45.1436, "24/7 (Version 1)"},
    {21, 5, 454.9853, -107.2548, 999.4376, 309.0195, "Diner 1"},
    {22, 5, 372.5565, -131.3607, 1001.4922, 354.2285, "Pizza Stack"},
    {23, 17, 378.0260, -190.5155, 1000.6328, 141.0245, "Rusty Brown's Donuts"},
    {24, 7, 315.2440, -140.8858, 999.6016, 7.4226, "Ammu-Nation"},
    {25, 5, 225.0306, -9.1838, 1002.2180, 85.5322, "Victim"},
    {26, 2, 611.3536, -77.5574, 997.9995, 320.9263, "Loco Low Co"},
    {27, 10, 246.0688, 108.9703, 1003.2188, 0.2922, "San Fierro Police Department"},
    {28, 10, 6.0856, -28.8966, 1003.5494, 5.0365, "24/7 (Version 2 - Large)"},
    {29, 7, 773.7318, -74.6957, 1000.6542, 5.2304, "Below The Belt Gym"},
    {30, 1, 621.4528, -23.7289, 1000.9219, 15.6789, "Transfenders"},
    {31, 31, 445.6003, -6.9823, 1000.7344, 172.2105, "World of Coq"},
    {32, 1, 285.8361, -39.0166, 1001.5156, 0.7529, "Ammu-Nation (Version 2)"},
    {33, 1, 204.1174, -46.8047, 1001.8047, 357.5777, "SubUrban"},
    {34, 1, 245.2307, 304.7632, 999.1484, 273.4364, "Denise's Bedroom"},
    {35, 3, 290.6230, 309.0622, 999.1484, 89.9164, "Helena's Barn"},
    {36, 5, 322.5014, 303.6906, 999.1484, 8.1747, "Barbara's Love Nest"},
    {37, 1, -2041.2334, 178.3969, 28.8465, 156.2153, "San Fierro Garage"},
    {38, 1, -1402.6613, 106.3897, 1032.2734, 105.1356, "Oval Stadium"},
    {39, 7, -1403.0116, -250.4526, 1043.5341, 355.8576, "8-Track Stadium"},
    {40, 2, 1207.5087, -3.6289, 1000.9219, 214.6596, "The Pig Pen"},
    {41, 10, 2016.1156, 1017.1541, 996.8750, 88.0055, "Four Dragons"},
    {42, 1, -741.8495, 493.0036, 1371.9766, 71.7782, "Liberty City"},
    {43, 2, 2447.8704, -1704.4509, 1013.5078, 314.5253, "Ryder's House"},
    {44, 1, 2527.0176, -1679.2076, 1015.4986, 260.9709, "Sweet's House"},
    {45, 10, -1129.8909, 1057.5424, 1346.4141, 274.5268, "RC Battlefield"},
    {46, 3, 2496.0549, -1695.1749, 1014.7422, 179.2174, "The Johnson House"},
    {47, 10, 366.0248, -73.3478, 1001.5078, 292.0084, "Burger Shot"},
    {48, 1, 2233.9363, 1711.8038, 1011.6312, 184.3891, "Caligula's Casino"},
    {49, 2, 269.6405, 305.9512, 999.1484, 215.6625, "Katie's Love Nest"},
    {50, 2, 414.2987, -18.8044, 1001.8047, 41.4265, "Barber Shop 2 (Reece's)"},
    {51, 2, 1.1853, -3.2387, 999.4284, 87.5718, "Angel Pine Trailer"},
    {52, 18, -30.9875, -89.6806, 1003.5469, 359.8401, "24/7 (Version 3)"},
    {53, 18, 161.4048, -94.2416, 1001.8047, 0.7938, "Zip"},
    {54, 2, -2638.8232, 1407.3395, 906.4609, 94.6794, "The Pleasure Domes"},
    {55, 2, 1267.8407, -776.9587, 1091.9063, 231.3418, "Madd Dogg's Mansion"},
    {56, 2, 2536.5322, -1294.8425, 1044.1250, 254.9548, "Big Smoke's Crack Palace"},
    {57, 5, 2350.1597, -1181.0658, 1027.9766, 99.1864, "Burning Desire Building"},
    {58, 1, -2158.6731, 642.0900, 1052.3750, 86.5402, "Wu-Zi Mu's"},
    {59, 10, 419.8936, 2537.1155, 10.0000, 67.6537, "Abandoned AC Tower"},
    {60, 14, 256.9047, -41.6537, 1002.0234, 85.8774, "Wardrobe/Changing Room"},
    {61, 14, 204.1658, -165.7678, 1000.5234, 181.7583, "Didier Sachs"},
    {62, 12, 1133.3500, -7.8462, 1000.6797, 165.8482, "Casino (Redsands West)"},
    {63, 14, -1420.4277, 1616.9221, 1052.5313, 159.1255, "Kickstart Stadium"},
    {64, 17, 488.4389, -11.4271, 1000.6797, 130.6844, "Club"},
    {65, 18, 1727.2853, -1642.9451, 20.2254, 172.4193, "Atrium"},
    {66, 16, -202.8420, -24.0325, 1002.2734, 252.8154, "Los Santos Tattoo Parlor"},
    {67, 5, 2233.6919, -1112.8107, 1050.8828, 8.6483, "Safe House Group 1"},
    {68, 6, 1211.2484, -1049.0234, 1000.2188, 170.9341, "Safe House Group 2"},
    {69, 9, 2319.1272, -1023.9562, 1050.2109, 167.3959, "Safe House Group 3"},
    {70, 10, 2261.0977, -1137.8833, 1050.6328, 266.8800, "Safe House Group 4"},
    {71, 17, -944.2402, 1886.1536, 5.0051, 179.8548, "Sherman Dam"},
    {72, 16, -24.6959, -130.3763, 1003.5469, 178.9616, "24/7 (Version 4)"},
    {73, 15, 2217.2810, -1150.5349, 1025.7969, 273.7328, "Jefferson Motel"},
    {74, 1, 1.5491, 23.3183, 1199.5938, 359.9054, "Jet Interior"},
    {75, 1, 681.6216, -451.8933, -25.6172, 166.1660, "The Welcome Pump"},
    {76, 3, 234.6087, 1187.8195, 1080.2578, 349.4844, "Burglary House X1"},
    {77, 2, 225.5707, 1240.0643, 1082.1406, 96.2852, "Burglary House X2"},
    {78, 1, 224.2880, 1289.1907, 1082.1406, 359.8680, "Burglary House X3"},
    {79, 5, 239.2819, 1114.1991, 1080.9922, 270.2654, "Burglary House X4"},
    {80, 15, 207.5219, -109.7448, 1005.1328, 358.6200, "Binco"},
    {81, 15, 295.1391, 1473.3719, 1080.2578, 352.9526, "4 Burglary Houses"},
    {82, 15, -1417.8927, 932.4482, 1041.5313, 0.7013, "Blood Bowl Stadium"},
    {83, 12, 446.3247, 509.9662, 1001.4195, 330.5671, "Budget Inn Motel Room"},
    {84, 0, 2306.3826, -15.2365, 26.7496, 274.4900, "Palomino Bank"},
    {85, 0, 2331.8984, 6.7816, 26.5032, 100.2357, "Palomino Diner"},
    {86, 0, 663.0588, -573.6274, 16.3359, 264.9829, "Dillimore Gas Station"},
    {87, 18, -227.5703, 1401.5544, 27.7656, 269.2978, "Lil' Probe Inn"},
    {88, 0, -688.1496, 942.0826, 13.6328, 177.6574, "Torreno's Ranch"},
    {89, 0, -1916.1268, 714.8617, 46.5625, 152.2839, "Zombotech - Lobby Area"},
    {90, 0, 818.7714, -1102.8689, 25.7940, 91.1439, "Crypt In LS Cemetery"},
    {91, 0, 255.2083, -59.6753, 1.5703, 1.4645, "Blueberry Liquor Store"},
    {92, 2, 446.6260, 1397.7380, 1084.3047, 343.9647, "Pair of Burglary Houses"},
    {93, 5, 227.3922, 1114.6572, 1080.9985, 267.4590, "Crack Den"},
    {94, 5, 227.7559, 1114.3844, 1080.9922, 266.2624, "Burglary House X11"},
    {95, 4, 261.1165, 1287.2197, 1080.2578, 178.9149, "Burglary House X12"},
    {96, 4, 291.7626, -80.1306, 1001.5156, 290.2195, "Ammu-Nation (Version 3)"},
    {97, 4, 449.0172, -88.9894, 999.5547, 89.6608, "Jay's Diner"},
    {98, 4, -27.8440, -26.6737, 1003.5573, 184.3118, "24/7 (Version 5)"},
    {99, 0, 2135.2004, -2276.2815, 20.6719, 318.5900, "Warehouse 3"},
    {100, 4, 306.1966, 307.8190, 1003.3047, 203.1354, "Michelle's Love Nest*"},
    {101, 10, 24.3769, 1341.1829, 1084.3750, 8.3305, "Burglary House X14"},
    {102, 1, 963.0586, 2159.7563, 1011.0303, 175.3130, "Sindacco Abattoir"},
    {103, 0, 2548.4807, 2823.7429, 10.8203, 270.6003, "K.A.C.C. Military Fuels Depot"},
    {104, 0, 215.1515, 1874.0579, 13.1406, 177.5538, "Area 69"},
    {105, 4, 221.6766, 1142.4962, 1082.6094, 184.9618, "Burglary House X13"},
    {106, 12, 2323.7063, -1147.6509, 1050.7101, 206.5352, "Unused Safe House"},
    {107, 6, 344.9984, 307.1824, 999.1557, 193.6430, "Millie's Bedroom"},
    {108, 12, 411.9707, -51.9217, 1001.8984, 173.3449, "Barber Shop"},
    {109, 4, -1421.5618, -663.8262, 1059.5569, 170.9341, "Dirtbike Stadium"},
    {110, 6, 773.8887, -47.7698, 1000.5859, 10.7161, "Cobra Gym"},
    {111, 6, 246.6695, 65.8039, 1003.6406, 7.9562, "Los Santos Police Department"},
    {112, 14, -1864.9434, 55.7325, 1055.5276, 85.8541, "Los Santos Airport"},
    {113, 4, -262.1759, 1456.6158, 1084.3672, 82.4590, "Burglary House X15"},
    {114, 5, 22.8610, 1404.9165, 1084.4297, 349.6158, "Burglary House X16"},
    {115, 5, 140.3679, 1367.8837, 1083.8621, 349.2372, "Burglary House X17"},
    {116, 3, 1494.8589, 1306.4800, 1093.2953, 196.0650, "Bike School"},
    {117, 14, -1813.2130, -58.0120, 1058.9641, 335.3199, "Francis International Airport"},
    {118, 16, -1401.0670, 1265.3706, 1039.8672, 178.6483, "Vice Stadium"},
    {119, 6, 234.2826, 1065.2290, 1084.2101, 4.3864, "Burglary House X18"},
    {120, 6, -68.5145, 1353.8485, 1080.2109, 3.5742, "Burglary House X19"},
    {121, 6, -2240.1028, 136.9730, 1035.4141, 269.0954, "Zero's RC Shop"},
    {122, 6, 297.1440, -109.8702, 1001.5156, 20.2254, "Ammu-Nation (Version 4)"},
    {123, 6, 316.5025, -167.6272, 999.5938, 10.3031, "Ammu-Nation (Version 5)"},
    {124, 15, -285.2511, 1471.1970, 1084.3750, 85.6547, "Burglary House X20"},
    {125, 6, -26.8339, -55.5846, 1003.5469, 3.9528, "24/7 (Version 6)"},
    {126, 6, 442.1295, -52.4782, 999.7167, 177.9394, "Secret Valley Diner"},
    {127, 2, 2182.2017, 1628.5848, 1043.8723, 224.8601, "Rosenberg's Office In Caligulas"},
    {128, 6, 748.4623, 1438.2378, 1102.9531, 0.6069, "Fanny Batter's Whore House"},
    {129, 8, 2807.3604, -1171.7048, 1025.5703, 193.7117, "Colonel Furhberger's"},
    {130, 9, 366.0002, -9.4338, 1001.8516, 160.5280, "Cluckin' Bell"},
    {131, 1, 2216.1282, -1076.3052, 1050.4844, 86.4280, "The Camel's Toe Safehouse"},
    {132, 1, 2268.5156, 1647.7682, 1084.2344, 99.7331, "Caligula's Roof"},
    {133, 2, 2236.6997, -1078.9478, 1049.0234, 2.5706, "Old Venturas Strip Casino"},
    {134, 3, -2031.1196, -115.8287, 1035.1719, 190.1877, "Driving School"},
    {135, 8, 2365.1089, -1133.0795, 1050.8750, 177.3947, "Verdant Bluffs Safehouse"},
    {136, 0, 1168.5120, 1360.1145, 10.9293, 196.5933, "Bike School"},
    {137, 9, 315.4544, 976.5972, 1960.8511, 359.6368, "Andromada"},
    {138, 10, 1893.0731, 1017.8958, 31.8828, 86.1044, "Four Dragons' Janitor's Office"},
    {139, 11, 501.9578, -70.5648, 998.7578, 171.5706, "Bar"},
    {140, 8, -42.5267, 1408.2300, 1084.4297, 172.0680, "Burglary House X21"},
    {141, 11, 2283.3118, 1139.3070, 1050.8984, 19.7032, "Willowfield Safehouse"},
    {142, 9, 84.9244, 1324.2983, 1083.8594, 159.5582, "Burglary House X22"},
    {143, 9, 260.7421, 1238.2261, 1084.2578, 84.3084, "Burglary House X23"},
    {144, 0, -1658.1656, 1215.0002, 7.2500, 103.9074, "Otto's Autos"},
    {145, 0, -1961.6281, 295.2378, 35.4688, 264.4891, "Wang Cars"},
    {146, 11, 2003.1178, 1015.1948, 33.0080, 351.5789, "Four Dragons' Managerial Suite"}
};

new E_ADMIN_LEVEL:g_AdminLevel[MAX_PLAYERS];
new bool:g_AdminDuty[MAX_PLAYERS];

// ====== AdminSession_Reset ======
stock AdminSession_Reset(playerid)
{
    g_AdminLevel[playerid] = ADMIN_LEVEL_NONE;
    g_AdminDuty[playerid] = false;
    return 1;
}

// ====== AdminSession_SetLevel ======
stock AdminSession_SetLevel(playerid, E_ADMIN_LEVEL:level)
{
    g_AdminLevel[playerid] = level;

    if (level == ADMIN_LEVEL_NONE)
    {
        g_AdminDuty[playerid] = false;
    }

    return 1;
}

// ====== AdminSession_GetLevel ======
stock E_ADMIN_LEVEL:AdminSession_GetLevel(playerid)
{
    return g_AdminLevel[playerid];
}

// ====== AdminSession_SetDuty ======
stock AdminSession_SetDuty(playerid, bool:isDuty)
{
    g_AdminDuty[playerid] = isDuty;
    return 1;
}

// ====== AdminSession_IsOnDuty ======
stock bool:AdminSession_IsOnDuty(playerid)
{
    return g_AdminDuty[playerid];
}

// ====== Admin_IsAuthorized ======
stock bool:Admin_IsAuthorized(playerid, E_ADMIN_LEVEL:minLevel)
{
    if (!AccountSession_IsAuthenticated(playerid))
    {
        SendErrorMessage(playerid, "You are not logged into an account.");
        return false;
    }

    if (AdminSession_GetLevel(playerid) < minLevel)
    {
        PermissionError(playerid);
        return false;
    }

    return true;
}

// ====== Admin_IsDutyAuthorized ======
stock bool:Admin_IsDutyAuthorized(playerid, E_ADMIN_LEVEL:minLevel)
{
    if (!Admin_IsAuthorized(playerid, minLevel))
    {
        return false;
    }

    if (!AdminSession_IsOnDuty(playerid))
    {
        Admin_SendAdminText(playerid, "You must use /aduty before using this admin action.");
        return false;
    }

    return true;
}

// ====== Admin_SendSyntaxText ======
stock Admin_SendSyntaxText(playerid, const syntaxText[])
{
    new message[96];

    format(message, sizeof(message), "USAGE: "WHITE"%s", syntaxText);
    SendClientMessageEx(playerid, X11_GREY_80, message);
    return 1;
}

// ====== Admin_SendAdminText ======
stock Admin_SendAdminText(playerid, const text[])
{
    new message[144];

    format(message, sizeof(message), "ADMIN: "WHITE"%s", text);
    SendClientMessageEx(playerid, X11_TOMATO, message);
    return 1;
}

// ====== Admin_SendServerText ======
stock Admin_SendServerText(playerid, const text[])
{
    new message[144];

    format(message, sizeof(message), "SERVER: "WHITE"%s", text);
    SendClientMessageEx(playerid, X11_LIGHT_SKY_BLUE_1, message);
    return 1;
}

// ====== Admin_SendCommandList ======
stock Admin_SendCommandList(playerid)
{
    Admin_SendAdminText(playerid, "/adminlevel /ahelp /aduty");

    if (AdminSession_GetLevel(playerid) >= ADMIN_LEVEL_MODERATOR)
    {
        Admin_SendAdminText(playerid, "/spec /goto /gethere");
        Admin_SendAdminText(playerid, "Map marker teleport is available while on duty.");
    }

    if (AdminSession_GetLevel(playerid) >= ADMIN_LEVEL_HEAD_ADMIN)
    {
        Admin_SendAdminText(playerid, "/setadmin");
    }

    if (AdminSession_GetLevel(playerid) >= ADMIN_LEVEL_DEVELOPER)
    {
        Admin_SendAdminText(playerid, "/setinterior [interiorid] [variant]");
    }

    return 1;
}

// ====== Admin_ParsePlayerTarget ======
stock bool:Admin_ParsePlayerTarget(playerid, const params[], const syntaxText[], &targetid)
{
    if (!strlen(params))
    {
        Admin_SendSyntaxText(playerid, syntaxText);
        return false;
    }

    for (new i = 0; params[i] != '\0'; i++)
    {
        if (params[i] < '0' || params[i] > '9')
        {
            Admin_SendSyntaxText(playerid, syntaxText);
            return false;
        }
    }

    targetid = strval(params);

    if (targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid))
    {
        SendErrorMessage(playerid, "Invalid target player.");
        return false;
    }

    return true;
}

// ====== Admin_ParseSetAdminParams ======
stock bool:Admin_ParseSetAdminParams(playerid, const params[], &targetid, &level)
{
    new splitIndex;
    new targetText[16];
    new levelText[16];

    if (!strlen(params))
    {
        SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
        return false;
    }

    splitIndex = strfind(params, " ", true);

    if (splitIndex == -1)
    {
        SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
        return false;
    }

    strmid(targetText, params, 0, splitIndex, sizeof(targetText));
    strmid(levelText, params, splitIndex + 1, strlen(params), sizeof(levelText));

    if (!strlen(targetText) || !strlen(levelText))
    {
        SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
        return false;
    }

    for (new i = 0; targetText[i] != '\0'; i++)
    {
        if (targetText[i] < '0' || targetText[i] > '9')
        {
            SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
            return false;
        }
    }

    for (new i = 0; levelText[i] != '\0'; i++)
    {
        if (levelText[i] < '0' || levelText[i] > '9')
        {
            SendSyntaxMessage(playerid, "/setadmin [playerid] [level]");
            return false;
        }
    }

    targetid = strval(targetText);
    level = strval(levelText);

    return true;
}

// ====== Admin_ParseInteriorId ======
stock bool:Admin_ParseInteriorId(playerid, const params[], &interiorWorldId, &variantId)
{
    new splitIndex;
    new interiorIdText[16];
    new variantText[16];

    if (!strlen(params))
    {
        SendSyntaxMessage(playerid, "/setinterior [interiorid] [variant]");
        return false;
    }

    splitIndex = strfind(params, " ", true);

    if (splitIndex == -1)
    {
        SendSyntaxMessage(playerid, "/setinterior [interiorid] [variant]");
        return false;
    }

    strmid(interiorIdText, params, 0, splitIndex, sizeof(interiorIdText));
    strmid(variantText, params, splitIndex + 1, strlen(params), sizeof(variantText));

    if (!strlen(interiorIdText) || !strlen(variantText))
    {
        SendSyntaxMessage(playerid, "/setinterior [interiorid] [variant]");
        return false;
    }

    for (new i = 0; interiorIdText[i] != '\0'; i++)
    {
        if (interiorIdText[i] < '0' || interiorIdText[i] > '9')
        {
            SendSyntaxMessage(playerid, "/setinterior [interiorid] [variant]");
            return false;
        }
    }

    for (new i = 0; variantText[i] != '\0'; i++)
    {
        if (variantText[i] < '0' || variantText[i] > '9')
        {
            SendSyntaxMessage(playerid, "/setinterior [interiorid] [variant]");
            return false;
        }
    }

    interiorWorldId = strval(interiorIdText);
    variantId = strval(variantText);

    if (interiorWorldId < 0 || interiorWorldId > 31)
    {
        SendErrorMessage(playerid, "Interior id must be between 0 and 31.");
        return false;
    }

    if (variantId < 1)
    {
        SendErrorMessage(playerid, "Variant must be 1 or higher.");
        return false;
    }

    return true;
}

// ====== Admin_GetInteriorData ======
stock bool:Admin_GetInteriorData(interiorWorldId, variantId, &listEntryId, &Float:x, &Float:y, &Float:z, &Float:a, interiorName[], interiorNameSize)
{
    new currentVariant;

    for (new i = 0; i < sizeof(g_AdminInteriorData); i++)
    {
        if (g_AdminInteriorData[i][E_ADMIN_INTERIOR_WORLD_ID] != interiorWorldId)
        {
            continue;
        }

        currentVariant++;

        if (currentVariant != variantId)
        {
            continue;
        }

        listEntryId = g_AdminInteriorData[i][E_ADMIN_INTERIOR_COMMAND_ID];
        x = g_AdminInteriorData[i][E_ADMIN_INTERIOR_X];
        y = g_AdminInteriorData[i][E_ADMIN_INTERIOR_Y];
        z = g_AdminInteriorData[i][E_ADMIN_INTERIOR_Z];
        a = g_AdminInteriorData[i][E_ADMIN_INTERIOR_A];
        format(interiorName, interiorNameSize, "%s", g_AdminInteriorData[i][E_ADMIN_INTERIOR_NAME]);
        return true;
    }

    return false;
}

// ====== Admin_TeleportToInterior ======
stock bool:Admin_TeleportToInterior(playerid, interiorWorldId, variantId)
{
    new listEntryId;
    new Float:x;
    new Float:y;
    new Float:z;
    new Float:a;
    new interiorName[48];
    new message[144];

    if (!Admin_GetInteriorData(interiorWorldId, variantId, listEntryId, x, y, z, a, interiorName, sizeof(interiorName)))
    {
        SendErrorMessage(playerid, "Interior id and variant combination was not found.");
        return false;
    }

    TogglePlayerSpectating(playerid, false);
    SetPlayerInterior(playerid, interiorWorldId);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, a);

    format(
        message,
        sizeof(message),
        "Teleported to interior %d variant %d (%s, list %d).",
        interiorWorldId,
        variantId,
        interiorName,
        listEntryId
    );
    Admin_SendAdminText(playerid, message);
    return true;
}

// ====== Admin_OnGameModeInit ======
stock Admin_OnGameModeInit()
{
    return 1;
}

// ====== Admin_OnPlayerConnect ======
stock Admin_OnPlayerConnect(playerid)
{
    return AdminSession_Reset(playerid);
}

// ====== Admin_OnPlayerDisconnect ======
stock Admin_OnPlayerDisconnect(playerid, reason)
{
    #pragma unused reason
    return AdminSession_Reset(playerid);
}

// ====== Admin_OnPlayerClickMap ======
stock Admin_OnPlayerClickMap(playerid, Float:x, Float:y, Float:z)
{
    #pragma unused z

    if (!AccountSession_IsAuthenticated(playerid))
    {
        return 0;
    }

    if (AdminSession_GetLevel(playerid) < ADMIN_LEVEL_HELPER)
    {
        return 0;
    }

    if (!AdminSession_IsOnDuty(playerid))
    {
        return 0;
    }

    TogglePlayerSpectating(playerid, false);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPosFindZ(playerid, x, y, 999.0);
    Admin_SendAdminText(playerid, "Teleported to your map marker.");
    return 1;
}

// ====== Admin_LoadAccountLevel ======
stock Admin_LoadAccountLevel(playerid, accountId)
{
    new query[96];

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "SELECT `admin_level` FROM `accounts` WHERE `id` = %d LIMIT 1",
        accountId
    );
    mysql_tquery(Database_GetHandle(), query, "OnAdminAccountLevelLoaded", "dd", playerid, accountId);
    return 1;
}

// ====== Admin_SetAccountLevel ======
stock Admin_SetAccountLevel(accountId, level, assignedByAccountId)
{
    new query[128];
    #pragma unused assignedByAccountId

    mysql_format(
        Database_GetHandle(),
        query,
        sizeof(query),
        "UPDATE `accounts` SET `admin_level` = %d, `updated_at` = NOW() WHERE `id` = %d LIMIT 1",
        level,
        accountId
    );
    mysql_tquery(Database_GetHandle(), query);
    return 1;
}

// ====== OnAdminAccountLevelLoaded ======
public OnAdminAccountLevelLoaded(playerid, accountId)
{
    new levelValue;

    if (cache_num_rows() > 0)
    {
        cache_get_value_name_int(0, "admin_level", levelValue);
    }

    if (AccountSession_IsAuthenticated(playerid) && AccountSession_GetId(playerid) == accountId)
    {
        AdminSession_SetLevel(playerid, E_ADMIN_LEVEL:levelValue);
    }

    return 1;
}

#include "modules/admin/command/command.inc"
