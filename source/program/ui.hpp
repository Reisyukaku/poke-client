#pragma once

#include <map>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <unordered_map>
#include "imgui/imgui_nvn.h"
#include "nn/hid.h"
#include "nn/fs.hpp"
#include "UI/hexform.hpp"
#include "UI/infoform.hpp"
#include "UI/luaform.hpp"
#include "UI/settingsform.hpp"
#include "UI/aboutform.hpp"
#include "nn/mouse.hpp"
#include "nn/keyboard.hpp"
#include "filelogger.hpp"
#include "UI/themeManager.hpp"
#include "types.h"

class UI {
public:
    UI(std::string name, ImVec2 winSize);

	void Update();
	void Draw();
    void Render();
    
    void ToggleVisibility() {
        isVisible = !isVisible;
    }

    void ToggleDemo() {
        showDemo = !showDemo;
    }
    
    bool IsVisible() {
        return isVisible;
    }
	
private:
    pkcl::Mouse *mouse;
    pkcl::Keyboard *keyboard;
    pkcl::OffsetManager *offsetMan;
    std::string Name;
    ImVec2 WinSize;
    bool isVisible;
    bool showDemo;

    HexForm *hexForm = nullptr;
    InfoForm *infoForm = nullptr;
    LuaForm *luaForm = nullptr;
    SettingsForm *settingsForm = nullptr;
    AboutForm *aboutForm = nullptr;
};