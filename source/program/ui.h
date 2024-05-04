#pragma once

#include <map>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <unordered_map>
#include "imgui/imgui_nvn.h"
#include "imgui/font.h"
#include "nn/hid.h"
#include "nn/fs.hpp"
#include "UI/hexform.hpp"
#include "UI/infoform.hpp"
#include "UI/luaform.hpp"
#include "UI/settingsform.hpp"
#include "nn/mouse.hpp"
#include "nn/keyboard.hpp"
#include "filelogger.hpp"
#include "types.h"

class UI {
public:
    void Initialize(std::string name, ImVec2 winSize);
	void Update();
	void Draw();
    void Render();
    
    void AddMenuButton(std::string name, std::function<void()> func);
    
    void ToggleVisibility() {
        isVisible = !isVisible;
    }
    
    bool IsVisible() {
        return isVisible;
    }
    
    void RowSelectUp() {
        selectedRow--;
    }
    
    void RowSelectDown() {
        selectedRow++;
    }
    
    void SetSelectRow(uint32_t r) {
        selectedRow = r;
    }
    
    uint32_t GetSelectRow() {
        return selectedRow;
    }
    
    void AddLogs(std::vector<std::string> logs);
	
private:
    std::string selectedStringPrefix(int cur);
    void WriteMenuItem(std::string str, uint32_t currRow);
	std::vector<u32> ks;
    exl::Mouse *mouse;
    exl::Keyboard *keyboard;
    exl::OffsetManager *offsetMan;
    std::string Name;
    ImVec2 WinSize;
    uint32_t selectedRow;
    bool isVisible;
    ImGuiStyle* style;

    HexForm *hexForm = nullptr;
    InfoForm *infoForm = nullptr;
    LuaForm *luaForm = nullptr;
    SettingsForm *settingsForm = nullptr;
};