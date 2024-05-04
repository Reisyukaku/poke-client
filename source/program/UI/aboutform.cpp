#include "aboutform.hpp"

AboutForm * AboutForm::instance = nullptr;

void AboutForm::Initialize()
{
	//
}

void AboutForm::Draw() {
    ImGui::SetNextWindowSize(ImVec2(500.0f, 250.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar)){
		ImGui::End();
		return;
	}
    
    ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    ImGui::Text("Poke-Client created by Rei");
	ImGui::EndChild();
    

    
	ImGui::End();
}