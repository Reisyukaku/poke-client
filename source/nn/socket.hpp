#pragma once

#include "types.h"
#include "nn/result.h"

struct in_addr
{
    u32 data;           // 0
};

struct sockaddr
{
    u8 _0;              // 0
    u8 family;          // 1
    u16 port;           // 2
    in_addr address;    // 4
    u8 _8[8];           // 8
};

struct hostent
{
    char*   h_name;
    char**  h_aliases;
    int     h_addrtype;
    int     h_length;
    char**  h_addr_list;
};


namespace nn
{
    namespace socket
    {
        struct Config {
            int unkInt1 = 2; // 0x0 (value is 2 in SMO sdk, 8 in sv)
            bool unkBool1 = false; // 0x4
            bool isUseBsdS = false; // 0x5
            void* pool; // 0x8
            ulong poolSize; // 0x10
            ulong allocPoolSize; // 0x18
            ulong unkLong1 = 0x8000; // 0x20
            ulong unkLong2 = 0x10000; // 0x28
            ulong unkLong3 = 0x30000; // 0x30
            ulong unkLong4 = 0x30000; // 0x38
            ulong unkLong5 = 0x2400; // 0x40
            ulong unkLong6 = 0xA500; // 0x48
            int unkInt3 = 4; // 0x50
            int concurLimit; // 0x54
            int padding;
        };
        nn::Result Initialize(Config const& config);
        nn::Result Initialize(void* pool, ulong poolSize, ulong allocPoolSize, int concurLimit);
        s32 SetSockOpt(s32 socket, s32 socketLevel, s32 option, void const*, u32 len);
        s32 Socket(s32 domain, s32 type, s32 protocol);
        s32 Connect(s32 socket,	const sockaddr* address, u32 addressLen);
        nn::Result Close(s32 socket);
        s32 Send(s32 socket, const void* data, ulong dataLen, s32 flags);
        s32 Recv(s32 socket, void* out, ulong outLen, s32 flags);
        u16 InetHtons(u16 val);
        s32 InetAton(const char* addressStr, in_addr* addressOut);
        struct hostent* GetHostByName(const char* name);
        u32 GetLastErrno();
    };
};