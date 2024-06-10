#include "game_hooks.h"

void game_hooks()
{
    switch(exl::setting::ProgramId) {
        case 0x01001F5010DFA000: //PLA
        {
            break;
        }
        case 0x0100ABF008968000: //Sword
        {
            break;
        }
        case 0x01008F6008C5E000: //Violet
        case 0x0100A3D008C5C000: //Scarlet
        {
            trpfd::InstallAtOffset(0xa17fe4);
            break;
        }
    }
}