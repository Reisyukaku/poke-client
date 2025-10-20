#include "luaform.hpp"
#include "debug.hpp"

LuaForm * LuaForm::instance = nullptr;

void LuaForm::LoadScriptList()
{
    DEBUG_LOG("Loading lua scripts\n");
    pkcl::FilesystemManager::getInstance()->TryMountSD();
    nn::fs::DirectoryHandle dirHandle;
    if(R_SUCCEEDED(nn::fs::OpenDirectory(&dirHandle, scriptDir.c_str(), nn::fs::OpenDirectoryMode_All))){
        nn::fs::GetDirectoryEntryCount(&scriptCnt, dirHandle);
        if(scriptCnt > 0){
            scriptList = new nn::fs::DirectoryEntry[scriptCnt];
            nn::fs::ReadDirectory(&scriptCnt, scriptList, dirHandle, scriptCnt);
        }
    }
    else
        DEBUG_LOG("Error: Failed to open lua script dir\n");
}

void LuaForm::ExecuteCmd(char *cmd)
{
    void* L = pkcl::LuaStateManager::getInstance()->GetLuaState();

    LuaH::loadBuffer(L, cmd, strlen(cmd), "poke-client VM", NULL);
    int ret = LuaH::pcallk(L, 0, LUA_MULTRET, 0, 0, 0);
    if(ret == LUA_OK){
        int nresults = LuaH::getTop(L);
        dbg_log.push_back(std::string("Returned ") + std::to_string(nresults) + std::string(" results."));
        for(int i = 0, j = -1 * nresults; i < nresults; i++, j++){
            int retType = LuaH::ltype(L, j);
            std::string res = pkcl::LuaStateManager::GetReturnTypeStr(retType) + std::string(": ");
            switch(retType){
                case LUA_TBOOLEAN:
                case LUA_TNUMBER:
                case LUA_TSTRING:
                case LUA_TTABLE:
                {
                    res += std::string(LuaH::toString(L, j, NULL));
                    break;
                }
                case LUA_TLIGHTUSERDATA:
                case LUA_TUSERDATA:
                {
                    void* ptr = LuaH::touserdata(L, j);
                    if(ptr != nullptr)
                    {
                        auto hex = Utils::ToHex((char*)ptr, 0x20); //sneak peak since no way to get size
                        DEBUG_LOG("%s\n", hex.c_str());
                        res += std::string("<logged>");
                    }
                    else res += std::string("failed to get");
                    break;
                }
                default:
                    res += std::string("<ommited>");
                    break;
            }
            dbg_log.push_back(res.c_str());
        }
        lua_Pop(L, nresults);
    }
    else
        dbg_log.push_back(std::string("Error in lua:\n") + std::string(LuaH::toString(L, -1, NULL)));
}

void LuaForm::ExecuteFile(std::string file) 
{
    
    nn::fs::FileHandle lua;
    long sz;
    nn::fs::OpenFile(&lua, file.c_str(), nn::fs::OpenMode_Read);
    nn::fs::GetFileSize(&sz, lua);
    char buffer[sz + 1] = {0};
    nn::fs::ReadFile(lua, 0, buffer, sz);
    nn::fs::CloseFile(lua);
    
    LuaForm::ExecuteCmd(buffer);
}

void LuaForm::DumpClosures()
{
    //
}

void LuaForm::Draw() 
{
    const float bottom_elem = ImGui::GetFrameHeightWithSpacing() + 30;
    const float dbg_elem = 150;

    ImGui::SetNextWindowSize(ImVec2(800.0f, 400.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(Name.c_str(), &isVisible, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar)){
		ImGui::End();
		return;
	}

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Dump closures")) 
            {
                DumpClosures();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Spacing();

    ImGui::BeginGroup();
    ImGui::Text("Output");
    
    ImGui::BeginChild("##scrolling_out", ImVec2(0, -(bottom_elem + dbg_elem)), ImGuiChildFlags_Border);
	for(auto &l : out_log)
        ImGui::Text(l.c_str());
        
	ImGui::EndChild();
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::Text("Debug");
    
    ImGui::BeginChild("##scrolling_dbg", ImVec2(0, -bottom_elem), ImGuiChildFlags_Border);
	for(auto &l : dbg_log)
        ImGui::Text(l.c_str());
        
	ImGui::EndChild();
    ImGui::EndGroup();

    ImGui::BeginChild("##footer", ImVec2(0, 0), ImGuiWindowFlags_NoScrollbar);

    ImGui::SetNextItemWidth(-1);
    bool isEnter = ImGui::IsKeyPressed(ImGuiKey_Enter);
    static char term[100] = {0};
    ImGui::InputText("##console", term, IM_ARRAYSIZE(term));
    if(isEnter)
    {
        ExecuteCmd(term);
        memset(term, 0, 100);
    }

    if(ImGui::Button("Run Selected Script", ImVec2(220, 0))) {
        if(scriptCnt > 0 && selectedScript != "")
            ExecuteFile(scriptDir + selectedScript);
    }
    ImGui::SameLine();
    if(ImGui::Button("Log", ImVec2(100, 0))) {
        for(auto &l : out_log)
            pkcl::FileLogger::getInstance()->Log("[Lua][Output] %s\n", l.c_str());
        out_log.clear();

        for(auto &l : dbg_log)
            pkcl::FileLogger::getInstance()->Log("[Lua][Debug] %s\n", l.c_str());
        dbg_log.clear();
    }
    ImGui::SameLine();
    if(ImGui::Button("Clear", ImVec2(100, 0))) {
        out_log.clear();
        dbg_log.clear();
    }
    
    ImGui::SameLine();
    
    ImGui::SetNextItemWidth(-1);
    if (ImGui::BeginCombo("##combo", selectedScript.c_str())) 
    {
        if(scriptList != nullptr)
        {
            for (int n = 0; n < scriptCnt; n++) 
            {
                std::string file(scriptList[n].m_Name);
                bool isSelected = (selectedScript == file);
                if (ImGui::Selectable(file.c_str(), isSelected))
                    selectedScript = file;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::EndChild();

	ImGui::End();
}