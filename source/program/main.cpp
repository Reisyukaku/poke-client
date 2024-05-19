#include "lib.hpp"
#include "imgui/imgui_nvn.h"
#include "nn/hid.h"
#include "ui.hpp"
#include "offsetManager.hpp"
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

exl::Mouse *exl::Mouse::instance = nullptr;
exl::Keyboard *exl::Keyboard::instance = nullptr;
exl::FileLogger *exl::FileLogger::instance = nullptr;
exl::OffsetManager *exl::OffsetManager::instance = nullptr;

UI *ui;
exl::TcpLogger *sock = exl::TcpLogger::getInstance();

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

HOOK_DEFINE_TRAMPOLINE(trpfd) {
	static void Callback(void *unk) {
        char *p = *(char**)(*(unsigned long*)((long)unk+0x40)+0x38);
        if(sock->IsConnected()){
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
    //sock->init("192.168.1.158", 3080);
    
    ui = new UI();
    ui->Initialize("Poke-Client", {250.0f, 600.0f});
}

ImDrawData *nvnImguiCalc() {
	ui->Update();
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
    lua_hooks();
    //trpfd::InstallAtOffset(0xa17fe4);
    //test::InstallAtOffset(0x1352134); 
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}