#pragma once

#include "lib.hpp"
#include "lib/libsetting.hpp"
#include "tcplogger.hpp"

typedef struct {
    char unk_0x0[0x30];
    uint64_t hash; //???
    char *filename;
    uint64_t unk_0x40;
    char *filename2;
} unkStruct;

typedef struct {
    void *unk_0x0;
    char unk_0x8[0x38];
    unkStruct *unk_0x40;
} trp_struct;

HOOK_DEFINE_TRAMPOLINE(trpfd) {
	static void Callback(trp_struct *c) {
        pkcl::TcpLogger::PrintHex((char*)c->unk_0x40, 0x40);
        pkcl::TcpLogger::PrintString("%016llX|%s\n", c->unk_0x40->hash, c->unk_0x40->filename);
        Orig(c);
    }
};

void game_hooks();