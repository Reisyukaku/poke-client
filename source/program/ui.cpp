#include "ui.hpp"

UI::UI(std::string name, ImVec2 winSize) : Name(name), WinSize(winSize), isVisible(true)
{
    mouse = pkcl::Mouse::getInstance();
    keyboard = pkcl::Keyboard::getInstance();
    offsetMan = pkcl::OffsetManager::getInstance();

    hexForm = HexForm::getInstance();
    hexForm->SetAddr(offsetMan->GetAddr(0x43f5b38)); //(0x43811c0));

    infoForm = InfoForm::getInstance();
    luaForm = LuaForm::getInstance();
    settingsForm = SettingsForm::getInstance();
    aboutForm = AboutForm::getInstance();

    pkcl::ThemeManager::SetTheme(pkcl::ThemeManager::Themes::Default);
    //ImGui::SetNextWindowCollapsed(true);
}

void UI::Update() {
	mouse->Poll();
	keyboard->Poll();

	mouse->Update();
    keyboard->Update();
}

void UI::Draw() {    
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
    if(ImGui::Button(aboutForm->GetName().c_str(), buttonSize)) aboutForm->Open();
	
	if(hexForm->IsOpen()) hexForm->Draw();
    if(infoForm->IsOpen()) infoForm->Draw();
    if(luaForm->IsOpen()) luaForm->Draw();
    if(settingsForm->IsOpen()) settingsForm->Draw();
    if(aboutForm->IsOpen()) aboutForm->Draw();
    if(showDemo) ImGui::ShowDemoWindow();
    
    if(ImGui::IsKeyPressed(ImGuiKey_F1, false))
        ToggleDemo();
    
    ImGui::End();
}

void UI::Render() {
    ImGui::Render();
}