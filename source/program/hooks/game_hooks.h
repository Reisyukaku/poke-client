#pragma once

#include "lib.hpp"
#include "lib/libsetting.hpp"
#include "tcplogger.hpp"
#include "utils.hpp"

typedef struct {
    char unk_0x0[0x30];
    uint64_t hash; //???
    char *filename;
    uint64_t unk_0x40;
    char *filename2;
} someFileDesc;

typedef struct {
    void *unk_0x0;
    char unk_0x8[0x38];
    someFileDesc *fileDesc;
} trp_struct;

HOOK_DEFINE_TRAMPOLINE(trpfd) {
	static void Callback(trp_struct *c) {
        pkcl::TcpLogger::PrintHex((char*)c->fileDesc, 0x40);
        pkcl::TcpLogger::PrintString("%016llX|%s\n", c->fileDesc->hash, c->fileDesc->filename);
        /*char *path = strchr(c->fileDesc->filename, ':');
        if(path != NULL)
        {
            uint64_t hash = Utils::FNVA1Hash(path);
        }*/
        
        Orig(c);
    }
};

void game_hooks();