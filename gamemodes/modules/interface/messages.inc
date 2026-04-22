#if defined AVE_INTERFACE_MESSAGES_INC
    #endinput
#endif
#define AVE_INTERFACE_MESSAGES_INC

#define X11_LIGHT_SKY_BLUE_1      (0x87CEFAFF)
#define X11_GREY_80               (0xCCCCCCFF)
#define X11_TOMATO                (0xFF6347FF)
#define X11_OLD_GOLD              (0xCFB53BFF)
#define X11_OLD_GREEN             (0x9ACD32FF)

#define WHITE                     "{FFFFFF}"
#define LIGHT_BLUE                "{87CEFA}"
#define GREY                      "{CCCCCC}"
#define TOMATO                    "{FF6347}"
#define YELLOW                    "{CFB53B}"
#define GREEN                     "{9ACD32}"
#define RED                       "{FF6347}"

#define SendClientMessageEx       SendClientMessage

#define SendServerMessage(%0,%1)  SendClientMessageEx(%0, X11_LIGHT_SKY_BLUE_1, "SERVER: "WHITE""%1)
#define SendCustomMessage(%0,%1,%2) SendClientMessageEx(%0, X11_LIGHT_SKY_BLUE_1, %1": "WHITE""%2)
#define SendSyntaxMessage(%0,%1)  SendClientMessageEx(%0, X11_GREY_80, "USAGE: "WHITE""%1)
#define SendErrorMessage(%0,%1)   SendClientMessageEx(%0, X11_GREY_80, "ERROR: "WHITE""%1)
#define SendAdminAction(%0,%1)    SendClientMessageEx(%0, X11_TOMATO, "ADMIN: "WHITE""%1)
#define PermissionError(%0)       SendClientMessageEx(%0, X11_GREY_80, "ERROR: "WHITE"You don't have any permissions!")
