#pragma once

typedef int __fastcall (*_luaLoadbuffer)(void*, const char*, size_t, char*, char*);
typedef int __fastcall (*_luaPcall)(void*, int, int, int, uintptr_t, void*);
typedef char* __fastcall (*_luaToString)(void*, int, void*);
typedef void __fastcall (*_luaSetTop)(void*, int);
typedef int __fastcall (*_luaGetTop)(void*);
typedef void __fastcall (*_luaPushNum)(void*, double);
typedef void __fastcall (*_luaGetGlobal)(void*, char*);
typedef int __fastcall (*_luaLoadFile)(void*, const char *, const char*);
typedef void __fastcall (*_luapushstring)(void*, char*);
typedef int __fastcall (*_luaType)(void*, int);
static char luaTypes[][11] = {
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
