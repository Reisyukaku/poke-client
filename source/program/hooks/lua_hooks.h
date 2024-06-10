#pragma once

#include "../UI/infoform.hpp"
#include "offsetManager.hpp"
#include "luaStateManager.hpp"
#include "lib.hpp"
#include "tcplogger.hpp"

HOOK_DEFINE_TRAMPOLINE(luaprint) {
	static int Callback(void *L) {
        int nresults = LuaH::getTop(L);
        for(int i = 0, j = -1; i < nresults; i++)
            InfoForm::getInstance()->AddString(LuaH::toString(L, j--, NULL));
        lua_Pop(L, nresults);
        return 0;
    }
};

HOOK_DEFINE_TRAMPOLINE(luaNewState) {
	static void *Callback(void *L1, void *L2) {
        void *L = Orig(L1, L2);
        pkcl::LuaStateManager::getInstance()->SetLuaState(L);
        return L;
    }
};

HOOK_DEFINE_TRAMPOLINE(trscn) {
	static int Callback(void *L) {
        char * s = LuaH::toString(L, 2, NULL);
        pkcl::TcpLogger::PrintString("%s\n", s);
        return Orig(L);
    }
};

void lua_hooks();