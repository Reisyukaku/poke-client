#pragma once

#include <string>
#include <map>
#include "../types.h"

namespace pkcl {
class OffsetManager {
public:	
	void SetTitleID(u64 tid) {
        Title = tid;
    }
    
    void SetBaseAddr(uintptr_t addr) {
        BaseAddr = addr;
    }
    
    uintptr_t GetAddr(std::string offsetName) {
        return BaseAddr + GetOffset(offsetName);
    }
    
    uintptr_t GetAddr(uintptr_t offset) {
        return BaseAddr + offset;
    }
    
    uintptr_t GetOffset(std::string offsetName) {
        uintptr_t ret = 0;
        switch(Title) {
            case 0x01001F5010DFA000: ret = PLA_offsets[offsetName]; break;
            case 0x0100ABF008968000: ret = Sword_offsets[offsetName]; break;
            case 0x01008F6008C5E000:
            case 0x0100A3D008C5C000: ret = SV_offsets[offsetName]; break;
        }
        
        return ret;
    }

    static OffsetManager* getInstance() {
		if (instance == nullptr)
			instance = new OffsetManager();
		return instance;
	};

    OffsetManager(const OffsetManager&) = delete;
	OffsetManager& operator=(const OffsetManager&) = delete;
	
private:
    static OffsetManager* instance;

	OffsetManager() {
        BaseAddr = 0;
        Title = 0;
	}
    
    u64 Title;
    uintptr_t BaseAddr;	
    
    std::map<std::string, uintptr_t> PLA_offsets = { //1.0.0
        {"LuaNewState", 0x6cf00},                   //lua_newstate
        {"LuaLoadbuffer", 0x555e0},                 //luaL_loadbufferx
        {"LuaPCall", 0x52e10},                      //lua_pcallk
        {"LuaToString", 0x50da0},                   //lua_tostring
        {"LuaSetTop", 0x4ff10},                     //lua_settop
        {"LuaGetTop", 0x4fef0},                     //lua_gettop
        {"LuaType", 0},                             //lua_type
        {"LuaPrint", 0},                            //luaB_print
        {"LuaPanic", 0}                             //panic
    };
    
    std::map<std::string, uintptr_t> Sword_offsets = { //1.0.0
        {"LuaNewState", 0x1666940},
        {"LuaLoadbuffer", 0x165e160},
        {"LuaPCall", 0x1661c90},
        {"LuaToString", 0x165fd50},
        {"LuaSetTop", 0x165ef30},
        {"LuaGetTop", 0x165ef10},
        {"LuaType", 0x165f320},
        {"LuaPrint", 0},
        {"LuaPanic", 0}
    };
    
    std::map<std::string, uintptr_t> SV_offsets = { //1.0.0
        {"LuaNewState", 0xadae00},
        {"LuaLoadbuffer", 0xd69e70},
        {"LuaPCall", 0x99e3b0},
        {"LuaToString", 0x9aeb10},
        {"LuaSetTop", 0x99df80},
        {"LuaGetTop", 0x99df60},
        {"LuaType", 0xa4dd50},
        {"LuaPrint", 0x45f80},
        {"LuaPanic", 0x2374ce4}
    };
};
};