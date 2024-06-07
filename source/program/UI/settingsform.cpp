#include "settingsform.hpp"

SettingsForm * SettingsForm::instance = nullptr;

void SettingsForm::Draw() {
    ImGui::SetNextWindowSize(ImVec2(500.0f, 250.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar)){
		ImGui::End();
		return;
	}

	ImGui::BeginGroup();
    ImGui::Text("Options");

    ImGui::BeginChild("Options", ImVec2(0,0), true);
    ImGui::Checkbox("Wireframe Mode", &wireframeMode);
    ImGui::EndChild();

    ImGui::EndGroup();
    
	ImGui::End();
}