#pragma once

#include "../UI/luaform.hpp"
#include "offsetManager.hpp"
#include "luaStateManager.hpp"
#include "lib.hpp"
#include "tcplogger.hpp"

HOOK_DEFINE_TRAMPOLINE(luaprint) {
	static int Callback(lua_State *L) {
        int nresults = LuaH::getTop(L);
        for(int i = 0, j = -1; i < nresults; i++){
            std::string str = LuaH::toString(L, j--, NULL);
            LuaForm::getInstance()->AddOutString(str);
        }
        lua_Pop(L, nresults);
        return 0;
    }
};

HOOK_DEFINE_TRAMPOLINE(luapanic) {
	static void Callback(lua_State *L) {
        std::string str = LuaH::toString(L, -1, NULL);
        printf("[Lua][Panic] %s\n", str.c_str());
    }
};

static void luaWarn (void *ud, const char *message, int tocont);

HOOK_DEFINE_TRAMPOLINE(luaNewState) {
	static lua_State *Callback(lua_State *L1, lua_State *L2) {
        lua_State *L = Orig(L1, L2);
        //Set warning function
        (L->l_G)->ud_warn = L;
        (L->l_G)->warnf = luaWarn;
        pkcl::LuaStateManager::getInstance()->SetLuaState(L);
        return L;
    }
};

void lua_hooks();