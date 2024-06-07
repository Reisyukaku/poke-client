#pragma once

#include <string>
#include "../types.h"
#include "hooks/lua.h"

class LuaStateManager {
public:
	void *GetLuaState() {
        return luaState;
    }
    
    void SetLuaState(void *L) {
        luaState = L;
    }
    
    static LuaStateManager* getInstance() {
		if (instance == nullptr)
			instance = new LuaStateManager();
		return instance;
	};
	
private:    
	LuaStateManager() {
		luaState = nullptr;
	}
    
    void *luaState;
	
    static LuaStateManager* instance;
	LuaStateManager(const LuaStateManager&);
	LuaStateManager& operator=(const LuaStateManager&);
};