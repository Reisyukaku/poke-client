#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#define IMNVNFUNC __attribute__((visibility("default")))

// io.DisplaySize is fixed at this (not the actual viewport scale, its 1920x1080 in docked)
#define SCREEN_W 1280.0f
#define SCREEN_H 720.0f

//class NVNcommandBuffer;

typedef void* ImTextureID;

// From User Executable
extern "C" IMNVNFUNC void nvnImguiInitialize();
extern "C" IMNVNFUNC ImDrawData *nvnImguiCalc();
extern "C" IMNVNFUNC ImGuiIO &nvnImguiGetIO();
extern "C" IMNVNFUNC void nvnImguiFontGetTexDataAsAlpha8(unsigned char **out_pixels, int *out_width, int *out_height, int *out_bytes_per_pixel = (int *)0);