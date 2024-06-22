#include "game_hooks.h"

void game_hooks()
{
    //PLA
    #if EXL_PROGRAM_ID == 0x01001F5010DFA000

    #endif

    //Sword
    #if EXL_PROGRAM_ID == 0x01001F5010DFA000

    #endif

    //Scarlet/Violet
    #if EXL_PROGRAM_ID == 0x0100A3D008C5C000 || EXL_PROGRAM_ID == 0x01008F6008C5E000
    trpfd::InstallAtOffset(0xa17fe4);
    #endif
}