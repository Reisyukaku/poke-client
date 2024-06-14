#include "aboutform.hpp"

#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "?"
#endif

AboutForm *AboutForm::instance = nullptr;

void AboutForm::Draw() {
    ImGui::SetNextWindowSize(ImVec2(500.0f, 250.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar)){
		ImGui::End();
		return;
	}
    
    ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    ImGui::Text(
		"Poke-Client created by Reisyukaku\n\n"
		"Build: " GIT_COMMIT_HASH "\n\n"
		"Credits:\n"
		"- Exlaunch by shadowninja108\n"
	);
	ImGui::EndChild();

	ImGui::End();
}