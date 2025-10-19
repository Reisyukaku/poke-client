#include "lua_hooks.h"

void luaWarn (void *ud, const char *message, int tocont)
{
    DEBUG_LOG("[Lua][Warn] %s\n", message);
}

void lua_hooks()
{
    LuaH::Setup();
    auto offsetMan = pkcl::OffsetManager::getInstance();

    INSTALL_AT_OFFSET(luaNewState, offsetMan->GetOffset("LuaNewState"))
    INSTALL_AT_OFFSET(luaprint, offsetMan->GetOffset("LuaPrint"))
    INSTALL_AT_OFFSET(luapanic, offsetMan->GetOffset("LuaPanic"))
}