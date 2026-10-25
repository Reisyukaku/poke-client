#pragma once

#include "lib.hpp"
#include "flatbuffers/trpfd_generated.h"
#include "nn/fs.hpp"
#include <cstring>

HOOK_DEFINE_TRAMPOLINE(trpfd) {
#if EXL_PROGRAM_ID == 0x0100F43008C44000
    struct OpenResult {
        uint64_t valueAndFlags;
        uint64_t error;
    };

    template <typename T>
    static T& Field(void* object, size_t offset) {
        return *reinterpret_cast<T*>(static_cast<uint8_t*>(object) + offset);
    }

    static OpenResult Callback(void* file) {
        auto* string = Field<uint8_t*>(file, 0x40);
        if (!string) return Orig(file);
        const char* path = reinterpret_cast<const char*>(string +
            ((Field<uint32_t>(string, 8) & 2) ? 0x11 : 0x18));
        if (std::strncmp(path, "data:/", 6) != 0) return Orig(file);

        using TryOpen = OpenResult (*)(void*, const char*, int);
        auto tryOpen = reinterpret_cast<TryOpen>(exl::util::modules::GetTargetStart() + 0x1955458);
        auto* immediate = static_cast<uint8_t*>(file) + 0xd0;
        uint8_t previous[0x18];
        std::memcpy(previous, immediate, sizeof(previous));
        const auto result = tryOpen(immediate, path, nn::fs::OpenMode_Read);
        if ((result.valueAndFlags & 0xff00000000ULL) &&
            static_cast<uint32_t>(result.error) == 0) {
            Field<uint32_t>(file, 0x174) = 1;
            Field<uint64_t>(file, 0x1a8) = Field<uint64_t>(file, 0xd0);
            return result;
        }

        if (result.valueAndFlags & 1) nn::fs::CloseFile(Field<nn::fs::FileHandle>(file, 0xe0));
        std::memcpy(immediate, previous, sizeof(previous));
        return Orig(file);
    }
#else
    static void* Callback(trp_struct* file) {
        return Orig(file);
    }
#endif
};

void game_hooks();
