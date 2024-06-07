#include "luaform.hpp"

LuaForm * LuaForm::instance = nullptr;

void LuaForm::Run(std::string file) {    
    auto offsetMan = exl::OffsetManager::getInstance();

    lua_toString = reinterpret_cast<_luaToString>(offsetMan->GetAddr("LuaToString"));
    lua_setTop = reinterpret_cast<_luaSetTop>(offsetMan->GetAddr("LuaSetTop"));
    lua_getTop = reinterpret_cast<_luaGetTop>(offsetMan->GetAddr("LuaGetTop"));
    lua_loadBuffer = reinterpret_cast<_luaLoadbuffer>(offsetMan->GetAddr("LuaLoadbuffer"));
    lua_pcallk = reinterpret_cast<_luaPcall>(offsetMan->GetAddr("LuaPCall"));
    lua_ltype = reinterpret_cast<_luaType>(offsetMan->GetAddr("LuaType"));

    void* L = LuaStateManager::getInstance()->GetLuaState();
    
    nn::fs::FileHandle lua;
    long sz;
    nn::fs::OpenFile(&lua, file.c_str(), nn::fs::OpenMode_Read);
    nn::fs::GetFileSize(&sz, lua);
    char buffer[sz];
    memset(buffer, 0, sz);
    nn::fs::ReadFile(lua, 0, buffer, sz);
    nn::fs::CloseFile(lua);
    
    lua_loadBuffer(L, buffer, sz, "poke-client VM", NULL);
    int ret = lua_pcallk(L, 0, LUA_MULTRET, 0, 0, 0);
    if(!ret){
        int nresults = lua_getTop(L);
        Logs.push_back(std::string("Returned ") + std::to_string(nresults) + std::string(" results."));
        for(int i = 0, j = -1; i < nresults; i++){
            int retType = lua_ltype(L,j);
            std::string res = std::string(luaTypes[retType]) + std::string(": ");
            switch(retType){
                case 1: //Bool
                case 3: //Number
                case 4: //String
                {
                    res += std::string(lua_toString(L, j--, NULL));
                    break;
                }
                default:
                    res += std::string("<ommited> Type: " + std::string(luaTypes[retType]));
                    break;
            }
            Logs.push_back(res.c_str());
        }
        lua_Pop(L, nresults);
    }
    else
        Logs.push_back(std::string("Error in lua:\n") + std::string(lua_toString(L, -1, NULL)));
}

void LuaForm::Draw() {
    ImGui::SetNextWindowSize(ImVec2(800.0f, 400.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar)){
		ImGui::End();
		return;
	}
    
    ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
	for(auto &l : Logs)
        ImGui::Text(l.c_str());
        
	ImGui::EndChild();
    
    ImGui::Separator();
    
    if(ImGui::Button("Run Selected Script", ImVec2(220, 0))) {
        if(scriptCnt > 0 && selectedScript != "")
            Run(scriptDir + selectedScript);
    }
    ImGui::SameLine();
    if(ImGui::Button("Clear", ImVec2(220, 0))) {
        Logs.clear();
    }
    
    ImGui::SameLine();
    
    ImGui::SetNextItemWidth(300);
    if (ImGui::BeginCombo("##combo", selectedScript.c_str())) {
        for (int n = 0; n < scriptCnt; n++) {
            std::string file(scriptList[n].m_Name);
            bool isSelected = (selectedScript == file);
            if (ImGui::Selectable(file.c_str(), isSelected))
                selectedScript = file;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    
	ImGui::End();
}