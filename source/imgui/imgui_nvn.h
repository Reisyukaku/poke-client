#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#define IMNVNFUNC __attribute__((visibility("default")))

// io.DisplaySize is fixed at this (not the actual viewport scale, its 1920x1080 in docked)
#define SCREEN_W 1280.0f
#define SCREEN_H 720.0f

typedef void* ImTextureID;

bool nvnImguiInitialize();
ImDrawData *nvnImguiDraw();