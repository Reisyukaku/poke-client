#include "lua_hooks.h"

void lua_hooks()
{
    auto offsetMan = exl::OffsetManager::getInstance();
    luaNewState::InstallAtOffset(offsetMan->GetOffset("LuaNewState"));
    luaprint::InstallAtOffset(offsetMan->GetOffset("LuaPrint"));
}