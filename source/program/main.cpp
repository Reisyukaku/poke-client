#include "lib.hpp"
#include "imgui/imgui_nvn.h"
#include "nn/hid.h"
#include "ui.hpp"
#include "offsetManager.hpp"
#include "luaStateManager.hpp"
#include "nn/mouse.hpp"
#include "nn/keyboard.hpp"
#include "filelogger.hpp"
#include "tcplogger.hpp"
#include "UI/infoform.hpp"
#include "hooks/lua.h"
#include "utils.hpp"
#include <map>
#include <string>
#include <cstdio>
#include <vector>
#include <cstdarg>
#include <cstring>
#include "hooks/lua_hooks.h"
#include "hooks/nvn_hooks.h"

exl::Mouse *exl::Mouse::instance = nullptr;
exl::Keyboard *exl::Keyboard::instance = nullptr;
exl::OffsetManager *exl::OffsetManager::instance = nullptr;
LuaStateManager *LuaStateManager::instance = nullptr;

UI *ui;
exl::TcpLogger *sock = exl::TcpLogger::getInstance();
exl::FileLogger *fileLog = exl::FileLogger::getInstance();

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

HOOK_DEFINE_TRAMPOLINE(nnMainHook) {
    static void Callback()
    {
        //Init loggers
        fileLog->init();
        sock->init(TCPLOG_IP, 3080);
        Orig();
    }
};

/*HOOK_DEFINE_TRAMPOLINE(trpfd) {
	static void Callback(void *unk) {
        char *p = *(char**)(*(unsigned long*)((long)unk+0x40)+0x38);
        size_t sz=strlen(p);
        char buf[sz+2];
        buf[0] = 0;
        strcpy(buf, p);
        buf[sz] = '\n';
        buf[sz+1] = 0;
        exl::TcpLogger::PrintString(buf);
        Orig(unk);
        
    }
};*/

/*
* ---------------------------------------------
*/

bool nvnImguiInitialize()
{
    exl::TcpLogger::PrintString("Initializing UI\n");

    ui = new UI("Poke-Client", {250.0f, 600.0f});

    return true;
}

ImDrawData *nvnImguiDraw() {
	ui->Update();
	ui->Draw();
    ui->Render();
    
    return ui->IsVisible() ? ImGui::GetDrawData() : nullptr;
}

extern "C" void exl_main(void *x0, void *x1) {
    //Init hook env
    exl::hook::Initialize();
    
    //Setup offset manager before hooks are set
	auto offsetMan = exl::OffsetManager::getInstance();
    offsetMan->SetBaseAddr(exl::util::modules::GetTargetStart());
    offsetMan->SetTitleID(exl::setting::ProgramId);

    nnMainHook::InstallAtSymbol("nnMain");

	//Hooks
    nvn_hooks();
    lua_hooks();

    //trpfd::InstallAtOffset(0xa17fe4);
    
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}