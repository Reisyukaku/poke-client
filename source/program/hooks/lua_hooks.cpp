#include "lua_hooks.h"

void lua_hooks()
{
    auto offsetMan = exl::OffsetManager::getInstance();
    lua_toString = reinterpret_cast<_luaToString>(offsetMan->GetAddr("LuaToString"));
    lua_setTop = reinterpret_cast<_luaSetTop>(offsetMan->GetAddr("LuaSetTop"));
    lua_getTop = reinterpret_cast<_luaGetTop>(offsetMan->GetAddr("LuaGetTop"));
    lua_loadBuffer = reinterpret_cast<_luaLoadbuffer>(offsetMan->GetAddr("LuaLoadbuffer"));
    lua_pcallk = reinterpret_cast<_luaPcall>(offsetMan->GetAddr("LuaPCall"));
    lua_ltype = reinterpret_cast<_luaType>(offsetMan->GetAddr("LuaType"));

    luaNewState::InstallAtOffset(offsetMan->GetOffset("LuaNewState"));
    luaprint::InstallAtOffset(offsetMan->GetOffset("LuaPrint"));
    trscn::InstallAtOffset(0x2211020); 
}