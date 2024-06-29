#pragma once

#include <string>
#include <functional>
#include "imgui/imgui_nvn.h"
#include "filelogger.hpp"
#include "offsetManager.hpp"
#include "keyboard.hpp"
#include "luaStateManager.hpp"
#include "filesystemManager.hpp"
#include "nn/hid.h"
#include "nn/mouse.hpp"
#include "nn/fs.hpp"
#include "form.hpp"
#include "utils.hpp"
#include "types.h"

class LuaForm : public BasicForm{
public:
    
    void Draw() override;

    void LoadScriptList();

    void AddOutString(std::string str)
    {
        out_log.push_back(str);
    }
    
	static LuaForm* getInstance() {
		if (instance == nullptr)
			instance = new LuaForm();
		return instance;
	};

private:
    static LuaForm* instance;
	LuaForm(const LuaForm&);
	LuaForm& operator=(const LuaForm&);

    LuaForm() : scriptList(nullptr)
    {
        Name = "Lua Interface";
        scriptDir = "sd:/luaScripts/";
    }

    void ExecuteFile(std::string file);
    void ExecuteCmd(char *cmd);
    std::vector<std::string> out_log, dbg_log;
    s64 scriptCnt;
    std::string selectedScript;
    nn::fs::DirectoryEntry* scriptList;
    std::string scriptDir;
};