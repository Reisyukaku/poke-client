#include "ui.h"

void UI::WriteMenuItem(std::string str, uint32_t currRow) {
    if(currRow == selectedRow){
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,255,0,255));
        
        ImGui::Text(str.c_str());
        ImGui::PopStyleColor();
    }else
        ImGui::Text(str.c_str());
}

void UI::Initialize(std::string name, ImVec2 winSize) {
    Name = name;
	WinSize = winSize;
	hexForm = new HexForm();
	infoForm = new InfoForm();
    luaForm = new LuaForm();
    settingsForm = new SettingsForm();
    
    mouse = exl::Mouse::getInstance();
    keyboard = exl::Keyboard::getInstance();
    offsetMan = exl::OffsetManager::getInstance();
	exl::FileLogger::getInstance();
    
	hexForm->Initialize("Hex Viewer");
    hexForm->SetAddr(offsetMan->GetAddr(0x43811c0));
    
	infoForm->Initialize("Info");
    luaForm->Initialize("Lua");
    settingsForm->Initialize("Settings");
	
    ImGui::CreateContext();
    
    //Set font
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryCompressedTTF(LiberationMono_compressed_data, LiberationMono_compressed_size, 20.0f);
    io.MouseDrawCursor = true;
	ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	
    //Set menu select style
    style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    
    selectedRow = 0;
    isVisible = true;
    
    keyboard->SetMap();
    //ImGui::SetNextWindowCollapsed(true);
}

void UI::AddMenuButton(std::string name, UI::MenuItems func) {
    MenuOptions.push_back(std::tuple<std::string, UI::MenuItems>{name, func});
}

void UI::Update() {
	mouse->Poll();
	keyboard->Poll();
	
	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent((float)mouse->state.x, (float)mouse->state.y);
	io.AddMouseButtonEvent(0, mouse->state.buttons & nn::hid::MouseButtons::LeftClick);
	io.AddMouseButtonEvent(1, mouse->state.buttons & nn::hid::MouseButtons::RightClick);
}

//https://github.com/EwanMac3/splat2-test-mod-menu/blob/master/source/program/main.cpp
void UI::Draw() {
	ImGui::NewFrame();
    
    if(!ImGui::Begin(Name.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImGui::End();
        return;
    }
    
    ImVec2 res = ImVec2(WinSize.x, WinSize.y);
    ImGui::SetWindowSize(res);
    ImVec2 pos = ImVec2(0.0f, 20.0f);
    ImGui::SetWindowPos(pos);
	
	ImVec2 buttonSize = ImVec2(220, 40);
    int currentRow = 0;

    for (const auto &opt: MenuOptions) {
        switch(std::get<1>(opt)) {
			case HEX: {
				if(ImGui::Button(std::get<0>(opt).c_str(), buttonSize)) {
					hexForm->Open();
				}
				break;
			}
			case INFO: {
				if(ImGui::Button(std::get<0>(opt).c_str(), buttonSize)) {
					infoForm->Open();
				}
				break;
			}
            case LUA: {
				if(ImGui::Button(std::get<0>(opt).c_str(), buttonSize)) {
					luaForm->Open();
				}
				break;
			}
            case SETTINGS: {
				if(ImGui::Button(std::get<0>(opt).c_str(), buttonSize)) {
					settingsForm->Open();
				}
				break;
			}
			case DEBUG: {
				if(ImGui::Button(std::get<0>(opt).c_str(), buttonSize)) {
                    exl::FileLogger::getInstance()->Log(std::string("test"));
				}
				break;
			}
		}
        currentRow++;
    }
    keyboard->AddKeys();
	static char levelInput[128] = {};
	ImGui::InputTextMultiline("level", levelInput, IM_ARRAYSIZE(levelInput));
    
	if(hexForm->IsOpen()) hexForm->Draw();
	if(luaForm->IsOpen()) luaForm->Draw();
    if(infoForm->IsOpen()) infoForm->Draw();
    if(settingsForm->IsOpen()) settingsForm->Draw();
    
    ImGui::End();
}

void UI::Render() {
    ImGui::Render();
}