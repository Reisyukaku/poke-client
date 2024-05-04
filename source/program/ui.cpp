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
    
    mouse = exl::Mouse::getInstance();
    keyboard = exl::Keyboard::getInstance();
    offsetMan = exl::OffsetManager::getInstance();
	exl::FileLogger::getInstance();
	
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

    hexForm = HexForm::getInstance();
    hexForm->Initialize();
    hexForm->SetAddr(offsetMan->GetAddr(0x43811c0));

    infoForm = InfoForm::getInstance();
    infoForm->Initialize();

    luaForm = LuaForm::getInstance();
    luaForm->Initialize();

    settingsForm = SettingsForm::getInstance();
    settingsForm->Initialize();

    //ImGui::SetNextWindowCollapsed(true);
}

void UI::Update() {
	mouse->Poll();
	keyboard->Poll();

	mouse->Update();
    keyboard->Update();
}

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

    if(ImGui::Button(hexForm->GetName().c_str(), buttonSize)) hexForm->Open();
    if(ImGui::Button(infoForm->GetName().c_str(), buttonSize)) infoForm->Open();
    if(ImGui::Button(luaForm->GetName().c_str(), buttonSize)) luaForm->Open();
    if(ImGui::Button(settingsForm->GetName().c_str(), buttonSize)) settingsForm->Open();
	
	if(hexForm->IsOpen()) hexForm->Draw();
    if(infoForm->IsOpen()) infoForm->Draw();
    if(luaForm->IsOpen()) luaForm->Draw();
    if(settingsForm->IsOpen()) settingsForm->Draw();
    
    ImGui::End();
}

void UI::AddLogs(std::vector<std::string> logs){
    InfoForm::getInstance()->AddLogs(logs);
}

void UI::Render() {
    ImGui::Render();
}