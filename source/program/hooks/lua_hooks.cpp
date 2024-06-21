#include "lua_hooks.h"

void luaWarn (void *ud, const char *message, int tocont)
{
    printf("[Lua][Warn] %s\n", message);
}

void lua_hooks()
{
    LuaH::Setup();
    auto offsetMan = pkcl::OffsetManager::getInstance();
    luaNewState::InstallAtOffset(offsetMan->GetOffset("LuaNewState"));
    luaprint::InstallAtOffset(offsetMan->GetOffset("LuaPrint"));
    luapanic::InstallAtOffset(offsetMan->GetOffset("LuaPanic"));
}