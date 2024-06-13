#pragma once

#include <string.h>
#include "../../nvn/nvn_Cpp.h"
#include "lib.hpp"
#include "../imgui/imgui.h"
#include "tcplogger.hpp"
#include "UI/settingsform.hpp"

static nvn::DeviceInitializeFunc deviceInitPtr;
static nvn::DeviceGetProcAddressFunc procAddrPtr;

NVNboolean deviceInit(nvn::Device *device, const nvn::DeviceBuilder *builder);
nvn::GenericFuncPtrFunc deviceGetProc(nvn::Device *device, const char *procName);

HOOK_DEFINE_TRAMPOLINE(bootstrapLdr) {
	static void *Callback(const char *funcName) {

        void *result = Orig(funcName);

        if (strcmp(funcName, "nvnDeviceInitialize") == 0) {
            deviceInitPtr = (nvn::DeviceInitializeFunc) result;
            return (void *) &deviceInit;
        }
        if (strcmp(funcName, "nvnDeviceGetProcAddress") == 0) {
            procAddrPtr = (nvn::DeviceGetProcAddressFunc) result;
            return (void *) &deviceGetProc;
        }

        return result;
    }
};

void nvn_hooks();