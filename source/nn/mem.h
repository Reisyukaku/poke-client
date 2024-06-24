/**
 * @file mem.h
 * @brief Memory functions.
 */

#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void* memset(void *src, int val, u64 num);
void* memcpy(void *dest, void const *src, u64 count);
void* memmove( void* dest, const void* src, u64 count);
void* memalign(size_t alignment, size_t size);
void* memmem(void* needle, size_t needleLen, void* haystack, size_t haystackLen);

#ifdef __cplusplus
}
#endif

namespace nn::mem {

    class StandardAllocator {
        public:
            StandardAllocator();
            StandardAllocator(void* addr, size_t size);
            StandardAllocator(void* addr, size_t size, bool isCacheEnable);

            void Initialize(void *addr, u64 size);
            void Initialize(void* addr, size_t size, bool isCacheEnable);

            void Finalize();

            void *Reallocate(void *addr, u64 newSize);

            void *Allocate(size_t size);
            void *Allocate(size_t size, size_t alignment);

            void Free(void *addr);

            u64 GetSizeOf(const void *) const;

            u64 GetTotalFreeSize(void) const;

            void Dump();

            bool mIsInitialized;         // _0
            bool mIsEnabledThreadCache;  // _1
            u16 _2;
            u64 *mAllocAddr;  // _4
    };
}