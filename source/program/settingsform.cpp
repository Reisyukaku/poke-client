#include "settingsform.hpp"

void SettingsForm::Initialize(std::string name) {
	Name = name;
	isVisible = false;
}

void SettingsForm::Draw() {
    ImGui::SetNextWindowSize(ImVec2(500.0f, 250.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar)){
		ImGui::End();
		return;
	}
    
    ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    //
	ImGui::EndChild();
    

    
	ImGui::End();
}