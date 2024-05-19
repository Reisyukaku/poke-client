#pragma once

#include "../UI/infoform.hpp"
#include "offsetManager.hpp"
#include "lib.hpp"
#include "lua.h"

HOOK_DEFINE_TRAMPOLINE(luaprint) {
	static int Callback(void *L) {
        _luaToString toString = reinterpret_cast<_luaToString>(exl::OffsetManager::getInstance()->GetAddr("LuaToString"));
        _luaSetTop setTop = reinterpret_cast<_luaSetTop>(exl::OffsetManager::getInstance()->GetAddr("LuaSetTop"));
        _luaGetTop getTop = reinterpret_cast<_luaGetTop>(exl::OffsetManager::getInstance()->GetAddr("LuaGetTop"));
        int nresults = getTop(L);
        for(int i = 0, j = -1; i < nresults; i++)
            //if(sock->IsConnected()) 
                InfoForm::getInstance()->AddString(toString(L, j--, NULL));
        luaPop(L, nresults);
        return 0;
    }
};

HOOK_DEFINE_TRAMPOLINE(luaNewState) {
	static void *Callback(void *L1, void *L2) {
        void *L = Orig(L1, L2);
        exl::OffsetManager::getInstance()->SetLuaState(L);
        return L;
    }
};

void lua_hooks();