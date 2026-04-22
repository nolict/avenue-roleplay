#if defined AVE_INTERFACE_DIALOG_INC
    #endinput
#endif
#define AVE_INTERFACE_DIALOG_INC

#include <easyDialog>

stock Interface_FormatDialogHeader(dest[], size = sizeof(dest), const section[], const title[])
{
    format(dest, size, YELLOW"%s"WHITE" | "LIGHT_BLUE"%s", section, title);
    return 1;
}
