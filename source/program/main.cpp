#include "lib.hpp"
#include "imgui/imgui_nvn.h"
#include "nn/hid.h"
#include "ui.h"
#include "offsetManager.hpp"
#include "nn/mouse.hpp"
#include "nn/keyboard.hpp"
#include "filelogger.hpp"
#include "tcplogger.hpp"
#include "lua.h"
#include <map>
#include <string>
#include <cstdio>
#include <vector>
#include <cstdarg>
#include <cstring>

UI *ui;

std::vector<std::string> logs;

exl::Mouse *exl::Mouse::instance = nullptr;
exl::Keyboard *exl::Keyboard::instance = nullptr;
exl::FileLogger *exl::FileLogger::instance = nullptr;
exl::OffsetManager *exl::OffsetManager::instance = nullptr;

typedef struct {
    uint64_t unk;
    uint64_t FnvHash;
    char * path;
    uint64_t pathLen;
    uint64_t unk2;
    int fsType;
} FileStruct;
/*
* ---------------------------------------------
*/

bool isInit = false;
exl::TcpLogger *sock = exl::TcpLogger::getInstance();
int EndsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}
HOOK_DEFINE_TRAMPOLINE(test) {
	static void Callback(void *unk) {
        char *p = *(char**)(*(unsigned long*)(unk+0x40)+0x38);
        if(!isInit){
            sock->init("192.168.1.158", 3080);
            isInit = true;
        }
        else {
            size_t sz=strlen(p);
            char buf[sz+2];
            buf[0] = 0;
            strcpy(buf, p);
            buf[sz] = '\n';
            buf[sz+1] = 0;
            sock->sendMessage(buf);
        }
        Orig(unk);
        
    }
};

HOOK_DEFINE_TRAMPOLINE(luaprint) {
	static int Callback(void *L) {
        _luaToString toString = reinterpret_cast<_luaToString>(exl::OffsetManager::getInstance()->GetAddr("LuaToString"));
        _luaSetTop setTop = reinterpret_cast<_luaSetTop>(exl::OffsetManager::getInstance()->GetAddr("LuaSetTop"));
        _luaGetTop getTop = reinterpret_cast<_luaGetTop>(exl::OffsetManager::getInstance()->GetAddr("LuaGetTop"));

        int nresults = getTop(L);
        for(int i = 0, j = -1; i < nresults; i++)
            logs.push_back(toString(L, j--, NULL));
        luaPop(L, nresults);
        return 0;
    }
};

/*HOOK_DEFINE_TRAMPOLINE(trpak) {
	static void Callback(const char **file) {
        const char *str = (const char*)file[0];
        uintptr_t x8;
        asm volatile ("mov %0, X8" : "=r"(x8));
        
        auto s1 = std::string(str);
        std::string s2(".blua");
        if(s1.find(s2) != -1)
            logs.push_back(s1);
            
        asm volatile ("mov X8, %0" : "+r"(x8));
        Orig(file);
    }
};*/

HOOK_DEFINE_TRAMPOLINE(luaNewState) {
	static void *Callback(void *L1, void *L2) {
        void *L = Orig(L1, L2);
        exl::OffsetManager::getInstance()->SetLuaState(L);
        return L;
    }
};

/*
* ---------------------------------------------
*/

/* This method shouldn't be edited */
ImGuiIO &nvnImguiGetIO() {
    return ImGui::GetIO();
}

/* This method shouldn't be edited */
void nvnImguiFontGetTexDataAsAlpha8(unsigned char **out_pixels, int *out_width, int *out_height, int *out_bytes_per_pixel) {
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->GetTexDataAsAlpha8(out_pixels, out_width, out_height, out_bytes_per_pixel); // Make sure you have enough memory
}

/* This method is called during initialization. You can create your imgui context, add fonts etc. */
void nvnImguiInitialize() {
    
    ui = new UI();
    ui->Initialize("PokeClient by Rei", {250.0f, 600.0f});
	
    ui->AddMenuButton("Show HexViewer", UI::MenuItems::HEX);
    ui->AddMenuButton("Show Info", UI::MenuItems::INFO);
    ui->AddMenuButton("Show Lua", UI::MenuItems::LUA);
	ui->AddMenuButton("Debug", UI::MenuItems::DEBUG);
    ui->AddMenuButton("Settings", UI::MenuItems::SETTINGS);
}

ImDrawData *nvnImguiCalc() {
	ui->Update();
    if(logs.size() > 0){
        ui->AddLogs(logs);
        logs.clear();
    }
	ui->Draw();
    ui->Render();
    
    return ui->IsVisible() ? ImGui::GetDrawData() : nullptr;
}

extern "C" void exl_main(void *x0, void *x1) {

    /* Setup hooking enviroment. */
    envSetOwnProcessHandle(exl::util::proc_handle::Get());
	
	//Setup offset manager before hooks are set
	auto offsetMan = exl::OffsetManager::getInstance();
    offsetMan->SetBaseAddr(exl::util::modules::GetTargetStart());
    offsetMan->SetTitleID(exl::setting::ProgramId);
	
	//Initialize hook
    exl::hook::Initialize();
    
	//Hooks  
    luaNewState::InstallAtOffset(offsetMan->GetOffset("LuaNewState"));
    luaprint::InstallAtOffset(0x45f80);
    test::InstallAtOffset(0xa17fe4);
    
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}