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
typedef void* (*_luaTouserdata)(void*, int);

#define lua_Pop(L,n) LuaH::setTop(L, -(n)-1)
#define LUA_MULTRET -1

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define LUA_OK		0
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define LUA_ERRERR	5

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
		touserdata = reinterpret_cast<_luaTouserdata>(offsetMan->GetAddr("LuaToUserdata"));
	}
	static inline _luaToString toString = nullptr;
	static inline _luaSetTop setTop = nullptr;
	static inline _luaGetTop getTop = nullptr;
	static inline _luaLoadbuffer loadBuffer = nullptr;
	static inline _luaPcall pcallk = nullptr;
	static inline _luaType ltype = nullptr;
	static inline _luaTouserdata touserdata = nullptr;
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

	LuaStateManager(const LuaStateManager&) = delete;
	LuaStateManager& operator=(const LuaStateManager&) = delete;
	
private:
	static LuaStateManager* instance;
	
	LuaStateManager() {
		luaState = nullptr;
	}
    
    void *luaState;
};
}