#pragma once

#include <string.h>
#include "../../nvn/nvn_Cpp.h"
#include "../../nvn/nvn_CppMethods.h"
#include "lib.hpp"
#include "../imgui/imgui.h"
#include "tcplogger.hpp"
#include "offsetManager.hpp"
#include "UI/settingsform.hpp"
#include "UI/infoform.hpp"
#include "nn/nv.hpp"
#include "../../nvn/GfxSystem.hpp"

static nvn::DeviceInitializeFunc deviceInitPtr;
static nvn::DeviceGetProcAddressFunc procAddrPtr;

NVNboolean deviceInit(nvn::Device *device, const nvn::DeviceBuilder *builder);
nvn::GenericFuncPtrFunc deviceGetProc(nvn::Device *device, const char *procName);

void InitializeImguiBackend();
void nvn_hooks();

HOOK_DEFINE_TRAMPOLINE(bootstrapLdr) {
	static void *Callback(const char *funcName) {
        void *result = Orig(funcName);

        if (strcmp(funcName, "nvnDeviceInitialize") == 0) 
        {
            deviceInitPtr = (nvn::DeviceInitializeFunc) result;
            return (void *) &deviceInit;
        }
        if (strcmp(funcName, "nvnDeviceGetProcAddress") == 0) 
        {
            procAddrPtr = (nvn::DeviceGetProcAddressFunc) result;
            return (void *) &deviceGetProc;
        }

        return result;
    }
};

//Initialize devtool allocator for debug layer
HOOK_DEFINE_TRAMPOLINE(graphicsAlloc)
{
    static void Callback(nv::AllocateFunction alloc, nv::FreeFunction free, nv::ReallocateFunction realloc, void* ptr) 
    {
        printf("nv::SetGraphicsAllocator()\n");
        Orig(alloc, free, realloc, ptr);
        
        printf("nv::SetGraphicsDevtoolsAllocator()\n");
        nv::SetGraphicsDevtoolsAllocator(alloc, free, realloc, ptr);
    }
};