#pragma once

#include <string>
#include <functional>
#include "imgui/imgui_nvn.h"
#include "filelogger.hpp"
#include "offsetManager.hpp"
#include "nn/hid.h"
#include "nn/mouse.hpp"
#include "nn/fs.hpp"
#include "form.hpp"
#include "hooks/lua.h"
#include "types.h"

class LuaForm : public BasicForm{
public:
    
    void Draw() override;
    
	static LuaForm* getInstance() {
		if (instance == nullptr)
			instance = new LuaForm();
		return instance;
	};

private:
    static LuaForm* instance;
	LuaForm(const LuaForm&);
	LuaForm& operator=(const LuaForm&);

    LuaForm()
    {
        Name = "Lua Interface";

        logger = exl::FileLogger::getInstance();
        offsetMan = exl::OffsetManager::getInstance();
        scriptDir = "sd:/luaScripts/";
        
        scriptList = nullptr;
        nn::fs::DirectoryHandle dirHandle;
        if(!nn::fs::OpenDirectory(&dirHandle, scriptDir.c_str(), nn::fs::OpenDirectoryMode_All)){
            nn::fs::GetDirectoryEntryCount(&scriptCnt, dirHandle);
            if(scriptCnt > 0){
                scriptList = new nn::fs::DirectoryEntry[scriptCnt];
                nn::fs::ReadDirectory(&scriptCnt, scriptList, dirHandle, scriptCnt);
            }
        }
    }

    void Run(std::string file);
    std::vector<std::string> Logs;
    exl::FileLogger *logger;
    exl::OffsetManager *offsetMan;
    s64 scriptCnt;
    std::string selectedScript;
    nn::fs::DirectoryEntry* scriptList;
    std::string scriptDir;
};