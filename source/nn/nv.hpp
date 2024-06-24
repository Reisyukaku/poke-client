#pragma once

#include <types.h>

namespace nv {

    typedef void*(* AllocateFunction) (size_t size, size_t alignment, void *userPtr);
    typedef void(* FreeFunction) (void *addr, void *userPtr);
    typedef void*(* ReallocateFunction) (void *addr, size_t newSize, void *userPtr);

    void SetGraphicsAllocator(AllocateFunction alloc, FreeFunction free, ReallocateFunction realloc, void* ptr);
    void SetGraphicsDevtoolsAllocator(AllocateFunction alloc, FreeFunction free, ReallocateFunction realloc, void* ptr);

}  // namespace nn::nv