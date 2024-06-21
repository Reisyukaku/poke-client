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
        {"LuaNewState", 0x6cf00},
        {"LuaLoadbuffer", 0x555e0},
        {"LuaPCall", 0x52e10},
        {"LuaToString", 0x50da0},
        {"LuaSetTop", 0x4ff10},
        {"LuaGetTop", 0x4fef0},
        {"LuaType", 0},
        {"LuaPrint", 0},
        {"LuaPanic", 0}
    };
    
    std::map<std::string, uintptr_t> Sword_offsets = { //1.0.0
        {"LuaNewState", 0},
        {"LuaLoadbuffer", 0},
        {"LuaPCall", 0},
        {"LuaToString", 0},
        {"LuaSetTop", 0},
        {"LuaGetTop", 0},
        {"LuaType", 0},
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