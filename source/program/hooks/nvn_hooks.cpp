#include "nvn_hooks.h"
#include "../../nvn/nvn_CppFuncPtrImpl.h"
#include "../../imgui/imgui_nvn.h"
#include <nn/oe.hpp>
#include <nn/init.hpp>
#include "../../imgui_backend/imgui_impl_nvn.hpp"

nvn::Device *nvnDevice;
nvn::Queue *nvnQueue;
nvn::CommandBuffer *nvnCmdBuf;

nvn::CommandBufferInitializeFunc cmdBufInit_ptr;
nvn::QueueInitializeFunc queueInit_ptr;
nvn::QueuePresentTextureFunc presentTexture_ptr;
nvn::WindowSetCropFunc windowSetCrop_ptr;

nvn::PolygonStateSetPolygonModeFunc polyStateSetMode_ptr;

bool hasInitImGui = false;

static void* (*mallocFuncPtr)(size_t size);
static void (*freeFuncPtr)(void *ptr);

NVNboolean deviceInit(nvn::Device *device, const nvn::DeviceBuilder *builder) {
    NVNboolean result = deviceInitPtr(device, builder);
    nvnDevice = device;
    nvn::nvnLoadCPPProcs(nvnDevice, procAddrPtr);
    return result;
}

NVNboolean queueInit(nvn::Queue *queue, const nvn::QueueBuilder *builder) {
    NVNboolean result = queueInit_ptr(queue, builder);
    nvnQueue = queue;
    return result;
}

NVNboolean cmdBufInit(nvn::CommandBuffer *buffer, nvn::Device *device) {
    NVNboolean result = cmdBufInit_ptr(buffer, device);
    nvnCmdBuf = buffer;

    if (!hasInitImGui && (nvnDevice && nvnQueue && nvnCmdBuf)){
        ImGuiMemAllocFunc allocFunc = [](size_t size, void *user_data) {
            return mallocFuncPtr(size);
        };

        ImGuiMemFreeFunc freeFunc = [](void *ptr, void *user_data) {
            freeFuncPtr(ptr);
        };

        ImGui::SetAllocatorFunctions(allocFunc, freeFunc, nullptr);
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;

        ImguiNvnBackend::NvnBackendInitInfo initInfo = {
            .device = nvnDevice,
            .queue = nvnQueue,
            .cmdBuf = nvnCmdBuf
        };

        ImguiNvnBackend::InitBackend(initInfo);
        nvnImguiInitialize();

        hasInitImGui = true;
        printf("ImGui finished init!\n");
    }

    return result;
}

void setCrop(nvn::Window *window, int x, int y, int w, int h) {
    windowSetCrop_ptr(window, x, y, w, h);

    if (!hasInitImGui) return;

    ImVec2 &dispSize = ImGui::GetIO().DisplaySize;
    ImVec2 windowSize = ImVec2(w - x, h - y);

    if (dispSize.x != windowSize.x && dispSize.y != windowSize.y) {
        bool isDockedMode = nn::oe::GetOperationMode() == nn::oe::OperationMode_Docked;

        dispSize = windowSize;
        ImguiNvnBackend::updateProjection(windowSize);
        ImguiNvnBackend::updateScale(isDockedMode);

    }
}

void presentTexture(nvn::Queue *queue, nvn::Window *window, int texIndex) {
    if (hasInitImGui){
        ImguiNvnBackend::newFrame();
        ImGui::NewFrame();
        ImguiNvnBackend::renderDrawData(nvnImguiDraw());
    }

    presentTexture_ptr(queue, window, texIndex);
}

void polyStateSetMode(nvn::PolygonState *buf, nvn::PolygonMode mode) {
    polyStateSetMode_ptr(buf, SettingsForm::getInstance()->WireframeEnabled() ? nvn::PolygonMode::LINE : nvn::PolygonMode::FILL);
}

nvn::GenericFuncPtrFunc deviceGetProc(nvn::Device *device, const char *procName) {
    nvn::GenericFuncPtrFunc ptr = procAddrPtr(nvnDevice, procName);
    if (strcmp(procName, "nvnQueueInitialize") == 0) {
        queueInit_ptr = (nvn::QueueInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &queueInit;
    } else if (strcmp(procName, "nvnCommandBufferInitialize") == 0) {
        cmdBufInit_ptr = (nvn::CommandBufferInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &cmdBufInit;
    } else if (strcmp(procName, "nvnWindowSetCrop") == 0) {
        windowSetCrop_ptr = (nvn::WindowSetCropFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &setCrop;
    } else if (strcmp(procName, "nvnQueuePresentTexture") == 0) {
        presentTexture_ptr = (nvn::QueuePresentTextureFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &presentTexture;
    } else if(strcmp(procName, "nvnPolygonStateSetPolygonMode") == 0) {
        polyStateSetMode_ptr = (nvn::PolygonStateSetPolygonModeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &polyStateSetMode;
    }

    return ptr;
}

void nvn_hooks()
{
    nn::ro::LookupSymbol(reinterpret_cast<uintptr_t *>(&mallocFuncPtr), "malloc");
    nn::ro::LookupSymbol(reinterpret_cast<uintptr_t *>(&freeFuncPtr), "free");
    bootstrapLdr::InstallAtSymbol("nvnBootstrapLoader");
}