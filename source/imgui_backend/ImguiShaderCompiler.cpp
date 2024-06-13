
#include "ImguiShaderCompiler.h"
#include "nn/fs.hpp"
#include "nn/init.hpp"
#include "glslc/glslc.h"
#include "result.hpp"
#include <cstring>
#include <cstdio>

const char *vtx_src = 
"#version 450 core\n"

"layout (location = 0) in vec2 inPos;\n"
"layout (location = 1) in vec2 inUv;\n"
"layout (location = 2) in vec4 inColor;\n"

"layout (location = 0) out vec2 vtxUv;\n"
"layout (location = 1) out vec4 vtxColor;\n"

"layout (std140, binding = 0) uniform VertUBO {\n"
"    mat4 proj;\n"
"    int isUseSrgb;\n"
"} ubo;\n"

"void main() {\n"
"    gl_Position = ubo.proj * vec4(inPos, 0.0, 1.0);\n"
"    vtxUv       = inUv;\n"
"    vtxColor    = inColor;\n"
"}";

const char *fsh_src = 
"#version 450 core\n"

"layout (location = 0) in vec2 vtxUv;\n"
"layout (location = 1) in vec4 vtxColor;\n"

"layout (std140, binding = 0) uniform VertUBO {\n"
"    mat4 proj;\n"
"    int isUseSrgb;\n"
"} ubo;\n"

"layout (binding = 1) uniform sampler2D tex;\n"
"layout (location = 0) out vec4 outColor;\n"

"vec4 toLinear(vec4 sRGB)\n"
"{\n"
"    bvec3 cutoff = lessThan(sRGB.rgb, vec3(0.04045));\n"
"    vec3 higher = pow((sRGB.rgb + vec3(0.055))/vec3(1.055), vec3(2.4));\n"
"    vec3 lower = sRGB.rgb/vec3(12.92);\n"
"    return vec4(mix(higher, lower, cutoff), sRGB.a);\n"
"}\n"

"void main() {\n"
"    if (ubo.isUseSrgb == 1) {\n"
"        outColor = vtxColor * texture(tex, vtxUv);\n"
"    } else {\n"
"        outColor = toLinear(vtxColor) * texture(tex, vtxUv);\n"
"    }\n"
"}\n";

// list of every shader type nvn supports/glslc can compile (in the order of NVNshaderStage)

const char *shaderNames[] = {
        "VERTEX",
        "FRAGMENT",
        "GEOMETRY",
        "TESS_CONTROL",
        "TESS_EVALUATION",
        "COMPUTE",
};

extern "C" void *glslc_Alloc(size_t size, size_t alignment, void *user_data = nullptr) {
    pkcl::TcpLogger::PrintString("Allocating ptr with size: %x aligned by %x\n", size, alignment);
    return nn::init::GetAllocator()->Allocate(ALIGN_UP(size, alignment));
}

extern "C" void glslc_Free(void *ptr, void *user_data = nullptr) {
    pkcl::TcpLogger::PrintString("Freeing ptr: %p\n", ptr);
    nn::init::GetAllocator()->Free(ptr);
}

extern "C" void *glslc_Realloc(void *ptr, size_t size, void *user_data = nullptr) {
    pkcl::TcpLogger::PrintString("Reallocating ptr: %p to size: %x\n", ptr, size);
    return nn::init::GetAllocator()->Reallocate(ptr, size);
}

void NOINLINE ReadCompiledShader(GLSLCoutput *compileData) {

    pkcl::TcpLogger::PrintString("Shader Count: %d\n", compileData->numSections);

    for (int i = 0; i < (int)compileData->numSections; ++i) {
        if (compileData->headers[i].genericHeader.common.type != GLSLCsectionTypeEnum::GLSLC_SECTION_TYPE_GPU_CODE)
            continue;
        auto compInfo = &compileData->headers[i].gpuCodeHeader;

        pkcl::TcpLogger::PrintString("Shader Stage: %s\n", shaderNames[compInfo->stage]);
        pkcl::TcpLogger::PrintString("Section Size: %x\n", compInfo->common.size);
        pkcl::TcpLogger::PrintString("Control Offset: %x\n", compInfo->common.size + compInfo->controlOffset);
        pkcl::TcpLogger::PrintString("Control Size: %x\n", compInfo->controlSize);
        pkcl::TcpLogger::PrintString("Data Offset: %x\n", compInfo->common.size + compInfo->dataOffset);
        pkcl::TcpLogger::PrintString("Data Size: %x\n", compInfo->dataSize);

    }
}

void createPath(char *fullPath, const char *rootDir, const char *file, const char *ext) {
    sprintf(fullPath, "%s/%s%s", rootDir, file, ext);
}

CompiledData NOINLINE CreateShaderBinary(GLSLCoutput *compileData, const char *shaderName) {

    // TODO: make this work with more/less than 2 shaders

    size_t binarySize = 0;

    for (int i = 0; i < (int)compileData->numSections; ++i) {
        if (compileData->headers[i].genericHeader.common.type == GLSLCsectionTypeEnum::GLSLC_SECTION_TYPE_GPU_CODE) {
            binarySize = ALIGN_UP(binarySize + compileData->headers[i].genericHeader.common.size, 0x100);
        }
    }

    binarySize = ALIGN_UP(binarySize + 0x1000, 0x1000);

    u8 *rawDataBinary = (u8 *) compileData;

    u8 *binaryBuffer = (u8 *) glslc_Alloc(binarySize, 0x1000);
    memset(binaryBuffer, 0, binarySize);

    u32 curBinaryPos = 0x10;
    u32 headerInfo[4] = {};

    // place control sections first (as we dont need to align it)
    for (int i = 0; i < (int)compileData->numSections; ++i) {
        if (compileData->headers[i].genericHeader.common.type != GLSLCsectionTypeEnum::GLSLC_SECTION_TYPE_GPU_CODE)
            continue;

        auto compInfo = &compileData->headers[i].gpuCodeHeader;

        void *controlSection = rawDataBinary + (compInfo->common.dataOffset + compInfo->controlOffset);
        memcpy(binaryBuffer + curBinaryPos, controlSection, compInfo->controlSize);
        headerInfo[i] = curBinaryPos;
        curBinaryPos = ALIGN_UP(curBinaryPos + compInfo->controlSize, 0x100);
    }

    // place data sections next
    for (int i = 0; i < (int)compileData->numSections; ++i) {
        if (compileData->headers[i].genericHeader.common.type != GLSLCsectionTypeEnum::GLSLC_SECTION_TYPE_GPU_CODE)
            continue;
        auto compInfo = &compileData->headers[i].gpuCodeHeader;

        void *dataSection = rawDataBinary + (compInfo->common.dataOffset + compInfo->dataOffset);

        memcpy(binaryBuffer + curBinaryPos, dataSection, compInfo->dataSize);

        headerInfo[compileData->numSections + i] = curBinaryPos;
        curBinaryPos = ALIGN_UP(curBinaryPos + compInfo->dataSize, 0x100);
    }

    memcpy(binaryBuffer, headerInfo, sizeof(headerInfo));

    return {binaryBuffer, binarySize};

}

/*const char *GetShaderSource(const char *path) {
    nn::fs::FileHandle handle;

    //EXL_ASSERT(FsHelper::isFileExist(path), "Failed to Find File!");

    R_ABORT_UNLESS(nn::fs::OpenFile(&handle, path, nn::fs::OpenMode_Read))

    long size = 0;
    nn::fs::GetFileSize(&size, handle);
    char *sourceFile = (char *) glslc_Alloc(size + 1, 8);

    R_ABORT_UNLESS(nn::fs::ReadFile(handle, 0, sourceFile, size))

    nn::fs::CloseFile(handle);

    sourceFile[size] = '\0';

    return sourceFile;
}*/

bool ImguiShaderCompiler::CheckIsValidVersion(nvn::Device *device) {

    pkcl::TcpLogger::PrintString("Checking if GLSLC subsdk is on a valid version.\n");

    nn::gfx::detail::GlslcDll *glslcDll = nn::gfx::detail::GlslcDll::GetInstance();

    if (glslcDll->IsInitialized()) {
        auto versionInfo = glslcDll->GlslcGetVersion();
        pkcl::TcpLogger::PrintString("GLSLC Major Version: %d\n", versionInfo.apiMajor);
        pkcl::TcpLogger::PrintString("GLSLC Minor Version: %d\n", versionInfo.apiMinor);

        int minMajorVersion = 0;
        int maxMajorVersion = 0;
        int minMinorVersion = 0;
        int maxMinorVersion = 0;

        device->GetInteger(nvn::DeviceInfo::GLSLC_MIN_SUPPORTED_GPU_CODE_MAJOR_VERSION, &minMajorVersion);
        pkcl::TcpLogger::PrintString("NVN Api Min Major Version: %d\n", minMajorVersion);
        device->GetInteger(nvn::DeviceInfo::GLSLC_MAX_SUPPORTED_GPU_CODE_MAJOR_VERSION, &maxMajorVersion);
        pkcl::TcpLogger::PrintString("NVN Api Max Major Version: %d\n", maxMajorVersion);
        device->GetInteger(nvn::DeviceInfo::GLSLC_MIN_SUPPORTED_GPU_CODE_MINOR_VERSION, &minMinorVersion);
        pkcl::TcpLogger::PrintString("NVN Api Min Minor Version: %d\n", minMinorVersion);
        device->GetInteger(nvn::DeviceInfo::GLSLC_MAX_SUPPORTED_GPU_CODE_MINOR_VERSION, &maxMinorVersion);
        pkcl::TcpLogger::PrintString("NVN Api Max Minor Version: %d\n", maxMinorVersion);

        if (((int)versionInfo.apiMajor >= minMajorVersion && (int)versionInfo.apiMajor <= maxMajorVersion) &&
            ((int)versionInfo.apiMinor >= minMinorVersion && (int)versionInfo.apiMinor <= maxMinorVersion)) {
            pkcl::TcpLogger::PrintString("NVN Api supports GLSLC version!\n");
            return true;
        } else if (minMajorVersion == 1 && maxMajorVersion == 1) {
            pkcl::TcpLogger::PrintString("Unable to verify if version is valid. Continuing to use GLSLC.\n");
            return true;
        }

        pkcl::TcpLogger::PrintString("Current NVN Api is incompatible with supplied GLSLC binary!\nTry using a GLSLC binary from a different game.\n");
        return false;


    } else {
        pkcl::TcpLogger::PrintString("GLSLC Instance is not Initialized! Or unable to find function pointers.\n");
        return false;
    }

}

void ImguiShaderCompiler::InitializeCompiler() {

    nn::gfx::detail::GlslcDll *glslcDll = nn::gfx::detail::GlslcDll::GetInstance();

    pkcl::TcpLogger::PrintString("Setting Glslc Alloc funcs.\n");

    glslcDll->GlslcSetAllocator(glslc_Alloc, glslc_Free, glslc_Realloc, nullptr);

    pkcl::TcpLogger::PrintString("Funcs setup.\n");
}

CompiledData ImguiShaderCompiler::CompileShader(const char *shaderName) {

    nn::gfx::detail::GlslcDll *glslcDll = nn::gfx::detail::GlslcDll::GetInstance();

    pkcl::TcpLogger::PrintString("Running compiler for File(s): %s\n", shaderName);

    GLSLCcompileObject initInfo{};
    initInfo.options = glslcDll->GlslcGetDefaultOptions();

    if (!glslcDll->GlslcInitialize(&initInfo)) {
        pkcl::TcpLogger::PrintString("Unable to Init with info.\n");
        return {};
    } else {
        pkcl::TcpLogger::PrintString("GLSLC initialized!\n");
    }

    const char *shaders[6];
    NVNshaderStage stages[6];

    shaders[0] = vtx_src;
    stages[0] = NVNshaderStage::NVN_SHADER_STAGE_VERTEX;

    shaders[1] = fsh_src;
    stages[1] = NVNshaderStage::NVN_SHADER_STAGE_FRAGMENT;

    initInfo.input.sources = shaders;
    initInfo.input.stages = stages;
    initInfo.input.count = 2;

    if (!(shaders[0] && shaders[1])) {
        pkcl::TcpLogger::PrintString("Failed to load Shader Source(s). Unable to compile.\n");
        return {};
    }

    pkcl::TcpLogger::PrintString("Got Shaders. Attempting to Compile.\n");

    if (glslcDll->GlslcCompile(&initInfo)) {
        pkcl::TcpLogger::PrintString("Successfully Compiled Shaders!\n");
    } else {
        pkcl::TcpLogger::PrintString("%s", initInfo.lastCompiledResults->compilationStatus->infoLog);

        pkcl::TcpLogger::PrintString("Vert Shader Source:\n%s\n", shaders[0]);
        pkcl::TcpLogger::PrintString("Frag Shader Source:\n%s\n", shaders[1]);
    }

    glslcDll->Finalize(); // finalize compiler

    // free shader source buffers after compile finishes
    glslc_Free((void *) shaders[0]);
    glslc_Free((void *) shaders[1]);

    return CreateShaderBinary(initInfo.lastCompiledResults->glslcOutput, shaderName);

}