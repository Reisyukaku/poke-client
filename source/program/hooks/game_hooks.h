#pragma once

#include "lib.hpp"
#include "lib/libsetting.hpp"
#include "tcplogger.hpp"
#include "filelogger.hpp"
#include "debug.hpp"
#include "flatbuffers/trpfd_generated.h"
#include "flatbuffers/trpfs_generated.h"

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
    someFileDesc *fileDesc; //0x40
    char unk_0x48[0x68];
    void *fileStruct;       //0xB0
    char unk_0xB8[0x90];
    void *flatbuffer_blob;  //0x148
    Titan::FileSystem::TRPFD *rootTable; //0x150
} trp_struct;

//typedef void (*find_hash_trpfs_maybe)(void* p1, void* p2, uint64_t hash, char *b);
//typedef void* (*OpenFileAndHash)(void *file, char *path, int mode);

HOOK_DEFINE_TRAMPOLINE(trpfd) {
	static void *Callback(trp_struct *c) {
        pkcl::Debug::Backtrace(10);
        printf("%s\n", c->fileDesc->filename);
        //bool isUnk = false;
        //auto offsetMan = pkcl::OffsetManager::getInstance();
        //find_hash_trpfs_maybe find_hash_trpfs_maybe_ptr = offsetMan->GetAddr(0xa19394);
        /*OpenFileAndHash openFile = reinterpret_cast<OpenFileAndHash>(offsetMan->GetAddr(0xa1925c));
        //printf("%016llX | %s\n", c->fileDesc->hash, c->fileDesc->filename);
        //FUN_7100a18374(DAT_7104358910, &c->fileDesc);
        //pkcl::FileLogger::getInstance()->LogRaw((char*)c, 0x300);
        char *path = strchr(c->fileDesc->filename, ':') + 2;
        printf("%s\n", path);
        if(path != NULL)
        {
            uint64_t hash = Utils::FNVA1Hash(path);
            printf("Hash: %lX\n", hash);
            find_hash_trpfs_maybe_ptr(DAT_7104358910,local_70,hash, &isUnk);
            if(isUnk)
            {
                printf(Utils::ToHex((char*)c->flatbuffer_blob, 0x20).c_str());
                c->rootTable = (Titan::FileSystem::TRPFD*)Titan::FileSystem::GetTRPFD(c->flatbuffer_blob);
                auto root = c->rootTable;
                auto hashes = root->file_hashes();
                for(int i = 0; i < hashes->size(); i++)
                {
                    printf("%lX\n", hashes->Get(i));
                }
            }
            else
            {
                printf("Opening file from romfs\n");
                void *f = openFile(c->fileStruct, path, 1);
            }

            //TODO: binary search on file_hashes
            //TODO: if hash found, load from trpfs, else load from romfs
        }*/
        
        
        return Orig(c);
    }
};

void game_hooks();