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
#include "lua.h"
#include "types.h"

class LuaForm : public BasicForm{
public:
    
    void Initialize(std::string name) override;
    void Draw() override;
    
	
private:
    void Run(std::string file);
    std::vector<std::string> Logs;
    exl::FileLogger *logger;
    exl::OffsetManager *offsetMan;
    s64 scriptCnt;
    std::string selectedScript;
    nn::fs::DirectoryEntry* scriptList;
    std::string scriptDir;
};