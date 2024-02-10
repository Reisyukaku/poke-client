#pragma once

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
#define luaPop(L,n)		setTop(L, -(n)-1)
#define LUA_MULTRET -1
