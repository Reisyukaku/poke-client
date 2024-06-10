#include "lua_hooks.h"

void lua_hooks()
{
    LuaH::Setup();
    auto offsetMan = pkcl::OffsetManager::getInstance();
    luaNewState::InstallAtOffset(offsetMan->GetOffset("LuaNewState"));
    luaprint::InstallAtOffset(offsetMan->GetOffset("LuaPrint"));
    trscn::InstallAtOffset(0x2211020); 
}