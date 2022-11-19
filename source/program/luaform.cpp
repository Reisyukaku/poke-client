#include "luaform.hpp"

void LuaForm::Initialize(std::string name) {
	Name = name;
	isVisible = false;
    logger = exl::FileLogger::getInstance();
    offsetMan = exl::OffsetManager::getInstance();
}

void LuaForm::Run() {    
    _luaLoadbuffer loadBuffer = reinterpret_cast<_luaLoadbuffer>(offsetMan->GetAddr("LuaLoadbuffer"));
    _luaPcall pcallk = reinterpret_cast<_luaPcall>(offsetMan->GetAddr("LuaPCall"));
    _luaToString toString = reinterpret_cast<_luaToString>(offsetMan->GetAddr("LuaToString"));
    _luaSetTop setTop = reinterpret_cast<_luaSetTop>(offsetMan->GetAddr("LuaSetTop"));
    _luaGetTop getTop = reinterpret_cast<_luaGetTop>(offsetMan->GetAddr("LuaGetTop"));
    _luaType ltype = reinterpret_cast<_luaType>(offsetMan->GetAddr("LuaType"));
    
    void* L = offsetMan->GetLuaState();
    
    nn::fs::FileHandle lua;
    long sz;
    nn::fs::OpenFile(&lua, "sd:/test.lua", nn::fs::OpenMode_Read);
    nn::fs::GetFileSize(&sz, lua);
    char buffer[sz];
    memset(buffer, 0, sz);
    nn::fs::ReadFile(lua, 0, buffer, sz);
    nn::fs::CloseFile(lua);
    
    loadBuffer(L, buffer, sz, "vm", NULL);
    int ret = pcallk(L, 0, LUA_MULTRET, 0, 0, 0);
    if(!ret){
        int nresults = getTop(L);
        Logs.push_back(std::string("Returned ") + std::to_string(nresults) + std::string(" results."));
        for(int i = 0, j = -1; i < nresults; i++){
            int retType = ltype(L,j);
            std::string res = std::string(luaTypes[retType]) + std::string(": ");
            switch(retType){
                case 1:
                case 3:
                case 4:
                {
                    res += std::string(toString(L, j--, NULL));
                    break;
                }
                default:
                    res += std::string("<ommited>");
                    break;
            }
            Logs.push_back(res.c_str());
        }
        luaPop(L, nresults);
    }
    else
        Logs.push_back(std::string("Error in lua"));
}

void LuaForm::Draw() {
    ImGui::SetNextWindowSize(ImVec2(600.0f, 400.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar)){
		ImGui::End();
		return;
	}
    
    ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
	for(auto &l : Logs)
        ImGui::Text(l.c_str());
        
	ImGui::EndChild();
    
    ImGui::Separator();
    
    if(ImGui::Button("Run", ImVec2(220, 30))) {
        Run();
    }
    ImGui::SameLine();
    if(ImGui::Button("Clear", ImVec2(220, 30))) {
        Logs.clear();
    }
    
	ImGui::End();
}