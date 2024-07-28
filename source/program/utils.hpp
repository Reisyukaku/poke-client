#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <offsetManager.hpp>

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

    static int EndsWith(const char *str, const char *suffix)
    {
        if (!str || !suffix)
            return 0;
        size_t lenstr = strlen(str);
        size_t lensuffix = strlen(suffix);
        if (lensuffix >  lenstr)
            return 0;
        return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
    }

    static std::string ToHex(char *buffer, size_t size) {
        if(buffer == nullptr || size == 0) return nullptr;

        std::string buf;
        const char *hex = "0123456789ABCDEF";
        for(size_t i = 0; i < size; i++){
            int mod = i % 16;
            uint8_t num1 = buffer[i] & 0xF;
            uint8_t num2 = (buffer[i] >> 4) & 0xF;
            
            buf.push_back(hex[num2]);
            buf.push_back(hex[num1]);
            if(mod == 7) buf.push_back(' ');
            buf.push_back((mod == 15) ? '\n' : ' ');
        }
        buf.push_back('\n');

        return buf;
    }

    typedef struct stack_frame {
        struct stack_frame *fp;
        uint64_t lr;
    } stack_frame_t;
    static inline void Backtrace(int depth = 5)
    {
        stack_frame_t *fp;

        __asm volatile ("mov %0, x29" : "=r" (fp));

        printf("Stack trace:\n");
        for(int i = 0; i < depth; i++, fp = fp->fp)
            printf("  0x%016lX\n", fp->lr - pkcl::OffsetManager::getInstance()->GetBaseAddr());
    }
};