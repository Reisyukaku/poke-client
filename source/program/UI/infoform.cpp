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
    ImGui::Text(" Fragment shader interlock: %s", TOBOOL(nvnInfo.supportFragInter));
    ImGui::Text(" Advanaced blend modes: %s", TOBOOL(nvnInfo.supportAdvBlend));
    ImGui::Text(" ASTC formats: %s", TOBOOL(nvnInfo.supportASTC));
    ImGui::Text(" Conservative rasterization: %s", TOBOOL(nvnInfo.supportConservRaster));
    ImGui::Text(" DrawTexture: %s", TOBOOL(nvnInfo.supportDrawTex));
    ImGui::Text(" Fragment coverage to color: %s", TOBOOL(nvnInfo.supportFragCovToCol));
    ImGui::Text(" Imagess using texture handle: %s", TOBOOL(nvnInfo.supportImgWithTexHandle));
    ImGui::Text(" Min/Max filtering: %s", TOBOOL(nvnInfo.supportMinMaxFilter));
    ImGui::Text(" Post-depth coverage: %s", TOBOOL(nvnInfo.supportPostDepthCov));
    ImGui::Text(" Programmable sample locations: %s", TOBOOL(nvnInfo.supportSampleLoc));
    ImGui::Text(" Packaged texture data in sparse tiled form: %s", TOBOOL(nvnInfo.supportSparseTileTex));
    ImGui::Text(" Stencil8 format: %s", TOBOOL(nvnInfo.supportStencil8));
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