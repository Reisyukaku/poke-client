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
#include "utils.hpp"
#include <map>
#include <string>
#include <cstdio>
#include <vector>
#include <cstdarg>
#include <cstring>
#include "hooks/lua_hooks.h"
#include "hooks/nvn_hooks.h"
#include "hooks/game_hooks.h"

pkcl::Mouse *pkcl::Mouse::instance = nullptr;
pkcl::Keyboard *pkcl::Keyboard::instance = nullptr;
pkcl::OffsetManager *pkcl::OffsetManager::instance = nullptr;
pkcl::LuaStateManager *pkcl::LuaStateManager::instance = nullptr;

UI *ui;
pkcl::TcpLogger *sock = pkcl::TcpLogger::getInstance();
pkcl::FileLogger *fileLog = pkcl::FileLogger::getInstance();

HOOK_DEFINE_TRAMPOLINE(nnMainHook) {
    static void Callback()
    {
        //Init loggers
        fileLog->init();
        sock->init(TCPLOG_IP, 3080);
        Orig();
    }
};

bool nvnImguiInitialize()
{
    pkcl::TcpLogger::PrintString("Initializing UI\n");

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
	auto offsetMan = pkcl::OffsetManager::getInstance();
    offsetMan->SetBaseAddr(exl::util::modules::GetTargetStart());
    offsetMan->SetTitleID(exl::setting::ProgramId);

    nnMainHook::InstallAtSymbol("nnMain");

	//Hooks
    nvn_hooks();
    lua_hooks();
    game_hooks();
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}