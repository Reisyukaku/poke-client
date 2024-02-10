#pragma once

#include <stdint.h>
#include <string.h>

#define BLANK_STRING 0xCBF29CE484222645

class Utils {
public:
    static uint64_t FNVA1Hash(const char *str, uint64_t basis = BLANK_STRING){
        int len = strlen(str);
        const uint64_t fnv64Offset = basis;
        const uint64_t fnv64Prime = 0x00000100000001b3;
        uint64_t hash = fnv64Offset;

        for (int i = 0; i < len; i++)
        {
            hash = hash ^ str[i];
            hash *= fnv64Prime;
        }

        return hash;
    }
};