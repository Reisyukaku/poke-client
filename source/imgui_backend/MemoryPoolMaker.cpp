#include "MemoryPoolMaker.h"
#include "imgui_impl_nvn.hpp"

bool MemoryPoolMaker::createPool(nvn::MemoryPool *result, size_t size,
                                 const nvn::MemoryPoolFlags &flags) {

    auto bd = ImguiNvnBackend::getBackendData();

    size_t alignedSize = ALIGN_UP(size, 0x1000);
    void *poolPtr = IM_ALLOC(alignedSize);
    if (!poolPtr) return false;

    nvn::MemoryPoolBuilder memPoolBuilder{};
    memPoolBuilder.SetDefaults().SetDevice(bd->device).SetFlags(
                    flags)
            .SetStorage(poolPtr, alignedSize);

    if (!result->Initialize(&memPoolBuilder)) {
        IM_FREE(poolPtr);
        return false;
    }

    return true;
}