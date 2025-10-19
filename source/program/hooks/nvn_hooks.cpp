#include "nvn_hooks.h"
#include "../../nvn/nvn_CppFuncPtrImpl.h"
#include "../../imgui/imgui_nvn.h"
#include "../../imgui_backend/imgui_impl_nvn.hpp"
#include <nn/oe.hpp>
#include <nn/init.hpp>

pkcl::GfxSystem *gfx = pkcl::GfxSystem::getInstance();

nvn::Queue *nvnQueue;
nvn::CommandBuffer *nvnCmdBuf;

nvn::CommandBufferInitializeFunc cmdBufInit_ptr;
nvn::QueueInitializeFunc queueInit_ptr;

nvn::QueuePresentTextureFunc presentTexture_ptr;
nvn::WindowSetCropFunc windowSetCrop_ptr;

nvn::PolygonStateSetPolygonModeFunc polyStateSetMode_ptr;
nvn::DeviceBuilderSetFlagsFunc devSetFlag_ptr;

SettingsForm *settings = SettingsForm::getInstance();
pkcl::OffsetManager *offsetMan = pkcl::OffsetManager::getInstance();

bool hasInitImGui = false;

static void* (*mallocFuncPtr)(size_t alignment, size_t size) = nullptr;
static void* (*reallocFuncPtr)(void *ptr, size_t size) = nullptr;
static void (*freeFuncPtr)(void *ptr) = nullptr;

void DebugLayerCallback(NVNdebugCallbackSource source, NVNdebugCallbackType type, int id, NVNdebugCallbackSeverity severity, const char * message, void* pUser)
{
    DEBUG_LOG("NVN Debug Layer Callback:\n");
    DEBUG_LOG("  source:    0x%08x\n", source);
    DEBUG_LOG("  type:      0x%08x\n", type);
    DEBUG_LOG("  id:        0x%08x\n", id);
    DEBUG_LOG("  severity:  0x%08x\n", severity);
    DEBUG_LOG("  message:   %s\n",     message);

}

NVNboolean deviceInit(nvn::Device *device, const nvn::DeviceBuilder *builder) {
    DEBUG_LOG("nvnDeviceInitialize()\n");

    NVNboolean result = deviceInitPtr(device, builder);
    gfx->SetDevice(device);
    nvn::nvnLoadCPPProcs(gfx->GetDevice(), procAddrPtr);
#ifdef PKCL_ENABLE_NVN_DEBUG_LAYER
    gfx->GetDevice()->InstallDebugCallback((nvn::DebugCallbackFunc)DebugLayerCallback, NULL, false);
#endif
    pkcl::NvnInfo info;
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::API_MINOR_VERSION, &info.versionMin);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::API_MAJOR_VERSION, &info.versionMaj);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_DEBUG_LAYER, &info.supportDbg);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_PASSTHROUGH_GEOMETRY_SHADERS, &info.supportPassGeoShdr);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_FRAGMENT_SHADER_INTERLOCK, &info.supportFragInter);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_ADVANCED_BLEND_MODES, &info.supportAdvBlend);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_ASTC_FORMATS, &info.supportASTC);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_CONSERVATIVE_RASTER, &info.supportConservRaster);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_DRAW_TEXTURE, &info.supportDrawTex);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_FRAGMENT_COVERAGE_TO_COLOR, &info.supportFragCovToCol);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_IMAGES_USING_TEXTURE_HANDLES, &info.supportImgWithTexHandle);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_MIN_MAX_FILTERING, &info.supportMinMaxFilter);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_POST_DEPTH_COVERAGE, &info.supportPostDepthCov);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_SAMPLE_LOCATIONS, &info.supportSampleLoc);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_SPARSE_TILED_PACKAGED_TEXTURES, &info.supportSparseTileTex);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_STENCIL8_FORMAT, &info.supportStencil8);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_TARGET_INDEPENDENT_RASTERIZATION, &info.supportTargetIndepRaster);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_VIEWPORT_SWIZZLE, &info.supportViewSwizzle);
    gfx->GetDevice()->GetInteger(nvn::DeviceInfo::SUPPORTS_ZERO_FROM_UNMAPPED_VIRTUAL_POOL_PAGES, &info.supportZeroUnmapPool);
    InfoForm::getInstance()->SetNvnInfo(info);
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
    InitializeImguiBackend();
    return result;
}

void devSetFlag(nvn::DeviceBuilder *dev, int flags)
{
#ifdef PKCL_ENABLE_NVN_DEBUG_LAYER
    //Set max level for debug layer
    flags |= nvn::DeviceFlagBits::DEBUG_ENABLE;
    flags |= nvn::DeviceFlagBits::DEBUG_ENABLE_LEVEL_4;
#endif
    devSetFlag_ptr(dev, flags);
}

void InitializeImguiBackend()
{
    if (!hasInitImGui && (gfx->GetDevice() && nvnQueue && nvnCmdBuf)){
        DEBUG_LOG("Initializing ImGui backend\n");

        IMGUI_CHECKVERSION();

        ImGuiMemAllocFunc allocFunc = [](size_t size, void *user_data) {
            assert(mallocFuncPtr != nullptr);
            return mallocFuncPtr(0x1000, size);
        };

        ImGuiMemFreeFunc freeFunc = [](void *ptr, void *user_data) {
            assert(freeFuncPtr != nullptr);
            freeFuncPtr(ptr);
        };
        
        ImGui::SetAllocatorFunctions(allocFunc, freeFunc, nullptr);
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;

        ImguiNvnBackend::NvnBackendInitInfo initInfo = {
            .device = gfx->GetDevice(),
            .queue = nvnQueue,
            .cmdBuf = nvnCmdBuf
        };
        
        ImguiNvnBackend::InitBackend(initInfo);
        nvnImguiInitialize();

        hasInitImGui = true;
        DEBUG_LOG("ImGui finished init!\n");
    }
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
    polyStateSetMode_ptr(buf, settings->WireframeEnabled() ? nvn::PolygonMode::LINE : nvn::PolygonMode::FILL);
}

nvn::GenericFuncPtrFunc deviceGetProc(nvn::Device *device, const char *procName) {
    nvn::GenericFuncPtrFunc ptr = procAddrPtr(gfx->GetDevice(), procName);
    if (strcmp(procName, "nvnQueueInitialize") == 0) 
    {
        queueInit_ptr = (nvn::QueueInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &queueInit;
    } 
    if (strcmp(procName, "nvnCommandBufferInitialize") == 0) 
    {
        cmdBufInit_ptr = (nvn::CommandBufferInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &cmdBufInit;
    } 
    if (strcmp(procName, "nvnWindowSetCrop") == 0) 
    {
        windowSetCrop_ptr = (nvn::WindowSetCropFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &setCrop;
    } 
    if (strcmp(procName, "nvnQueuePresentTexture") == 0) 
    {
        presentTexture_ptr = (nvn::QueuePresentTextureFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &presentTexture;
    }
    if (strcmp(procName, "nvnDeviceBuilderSetFlags") == 0) 
    {
        devSetFlag_ptr = (nvn::DeviceBuilderSetFlagsFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &devSetFlag;
    }
    if(strcmp(procName, "nvnPolygonStateSetPolygonMode") == 0) 
    {
        polyStateSetMode_ptr = (nvn::PolygonStateSetPolygonModeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &polyStateSetMode;
    }
    if(strcmp(procName, "nvnDeviceInitialize") == 0) //Older SDKs use getProc instead of bootstraploader to get deviceInit
    {
        deviceInitPtr = (nvn::DeviceInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &deviceInit;
    }

    return ptr;
}

void nvn_hooks()
{
    nn::ro::LookupSymbol(reinterpret_cast<uintptr_t *>(&mallocFuncPtr), "aligned_alloc");
    nn::ro::LookupSymbol(reinterpret_cast<uintptr_t *>(&reallocFuncPtr), "realloc");
    nn::ro::LookupSymbol(reinterpret_cast<uintptr_t *>(&freeFuncPtr), "free");

    if(mallocFuncPtr == nullptr) 
        DEBUG_LOG("malloc() unable to be dynamically resolved!\n");

    if(reallocFuncPtr == nullptr) 
        DEBUG_LOG("realloc() unable to be dynamically resolved!\n");

    if(freeFuncPtr == nullptr) 
        DEBUG_LOG("free() unable to be dynamically resolved!\n");

    bootstrapLdr::InstallAtSymbol("nvnBootstrapLoader");
#ifdef PKCL_ENABLE_NVN_DEBUG_LAYER
    graphicsAlloc::InstallAtSymbol("_ZN2nv20SetGraphicsAllocatorEPFPvmmS0_EPFvS0_S0_EPFS0_S0_mS0_ES0_");
#endif
}