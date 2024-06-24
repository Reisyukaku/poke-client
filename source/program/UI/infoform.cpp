#include "infoform.hpp"

#define TOBOOL(x) x ? "True" : "False"

InfoForm * InfoForm::instance = nullptr;

void walkCall(void *obj, void *user)
{
    printf("IM WALKIN HERE!\n");
}

void InfoForm::Draw() {
    ImGui::SetNextWindowSize(ImVec2(560.0f, 250.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar)){
		ImGui::End();
		return;
	}

    ImGui::BeginChild("##info", ImVec2(0, -28), ImGuiChildFlags_Border);
    ImGui::BeginGroup();
    ImGui::Text("NVN Version");
    ImGui::Text(" Major: %d", nvnInfo.versionMaj);
    ImGui::Text(" Minor: %d", nvnInfo.versionMin);
    ImGui::EndGroup();

    ImGui::Separator();

    ImGui::BeginGroup();
    ImGui::Text("Device Support");
    ImGui::Text(" Debug layer: %s", TOBOOL(nvnInfo.supportDbg));
    ImGui::Text(" Passthrough geometry shaders: %s", TOBOOL(nvnInfo.supportPassGeoShdr));
    ImGui::EndGroup();
    ImGui::EndChild();
    
    ImGui::BeginChild("##footer", ImVec2(0, 0), ImGuiWindowFlags_NoScrollbar);
    if(ImGui::Button("Write to SD", ImVec2(180, 0))) {
        //TODO
    }
    ImGui::SameLine();
#ifdef PKCL_ENABLE_NVN_DEBUG_LAYER
    if(ImGui::Button("Test", ImVec2(180, 0))) {
        //pkcl::GfxSystem::getInstance()->GetDevice()->WalkDebugDatabase(nvn::DebugObjectType::COMMAND_BUFFER, (nvn::WalkDebugDatabaseCallbackFunc) walkCall, nullptr);
    }
#endif
    ImGui::EndChild();
	ImGui::End();
}