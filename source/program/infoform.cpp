#include "infoform.hpp"

void InfoForm::Initialize(std::string name) {
	Name = name;
	isVisible = false;
    logger = exl::FileLogger::getInstance();
}

void InfoForm::Draw() {
    ImGui::SetNextWindowSize(ImVec2(500.0f, 250.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar)){
		ImGui::End();
		return;
	}
    
    ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
	for(auto &l : Logs)
        ImGui::Text(l.c_str());
        
	ImGui::EndChild();
    
    ImGui::Separator();
    
    if(ImGui::Button("Write to SD", ImVec2(220, 30))) {
        logger->Log(Logs);
    }
    ImGui::SameLine();
    if(ImGui::Button("Clear", ImVec2(220, 30))) {
        Logs.clear();
    }
    
	ImGui::End();
}