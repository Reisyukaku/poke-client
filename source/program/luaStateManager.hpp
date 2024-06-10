#pragma once

#include <string>
#include "../types.h"

typedef int (*_luaLoadbuffer)(void*, const char*, size_t, const char*, char*);
typedef int (*_luaPcall)(void*, int, int, int, uintptr_t, void*);
typedef char* (*_luaToString)(void*, int, void*);
typedef void (*_luaSetTop)(void*, int);
typedef int (*_luaGetTop)(void*);
typedef void (*_luaPushNum)(void*, double);
typedef void (*_luaGetGlobal)(void*, char*);
typedef int (*_luaLoadFile)(void*, const char *, const char*);
typedef void (*_luapushstring)(void*, char*);
typedef int (*_luaType)(void*, int);

static const char luaTypes[][11] = {
    "nil",
    "boolean",
    "userdata",
    "number",
    "string",
    "table",
    "function",
    "userdata",
    "thread",
    "upvalue",
    "proto",
};
#define lua_Pop(L,n) LuaH::setTop(L, -(n)-1)
#define LUA_MULTRET -1

class LuaH {
public:
	static void Setup() {
		auto offsetMan = pkcl::OffsetManager::getInstance();
		toString = reinterpret_cast<_luaToString>(offsetMan->GetAddr("LuaToString"));
		setTop = reinterpret_cast<_luaSetTop>(offsetMan->GetAddr("LuaSetTop"));
		getTop = reinterpret_cast<_luaGetTop>(offsetMan->GetAddr("LuaGetTop"));
		loadBuffer = reinterpret_cast<_luaLoadbuffer>(offsetMan->GetAddr("LuaLoadbuffer"));
		pcallk = reinterpret_cast<_luaPcall>(offsetMan->GetAddr("LuaPCall"));
		ltype = reinterpret_cast<_luaType>(offsetMan->GetAddr("LuaType"));
	}
	static inline _luaToString toString = nullptr;
	static inline _luaSetTop setTop = nullptr;
	static inline _luaGetTop getTop = nullptr;
	static inline _luaLoadbuffer loadBuffer = nullptr;
	static inline _luaPcall pcallk = nullptr;
	static inline _luaType ltype = nullptr;
};

namespace pkcl {
class LuaStateManager {
public:
	void *GetLuaState() {
        return luaState;
    }
    
    void SetLuaState(void *L) {
        luaState = L;
    }

	static std::string GetReturnTypeStr(int retType) {
		return std::string(luaTypes[retType]);
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
}