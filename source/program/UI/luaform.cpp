#include "luaform.hpp"
#include "debug.hpp"

LuaForm * LuaForm::instance = nullptr;

void LuaForm::LoadScriptList()
{
    DEBUG_LOG("Loading lua scripts\n");
    scriptList.clear();
    scriptCnt = 0;
    selectedScript.clear();
    pkcl::FilesystemManager::getInstance()->TryMountSD();
    nn::fs::DirectoryHandle directory;
    if (nn::fs::OpenDirectory(&directory, scriptDir.c_str(), nn::fs::OpenDirectoryMode_All) != 0) {
        DEBUG_LOG("Error: Failed to open lua script dir\n");
        return;
    }
    struct DirectoryGuard {
        nn::fs::DirectoryHandle handle;
        ~DirectoryGuard() { nn::fs::CloseDirectory(handle); }
    } guard{directory};

    s64 count = 0;
    if (nn::fs::GetDirectoryEntryCount(&count, directory) != 0 || count <= 0) return;
    std::vector<nn::fs::DirectoryEntry> entries(static_cast<size_t>(count));
    s64 readCount = 0;
    if (nn::fs::ReadDirectory(&readCount, entries.data(), directory, count) != 0) return;
    if (readCount < 0 || readCount > count) return;
    entries.resize(static_cast<size_t>(readCount));
    scriptList.swap(entries);
    scriptCnt = readCount;
}

void LuaForm::ExecuteCmd(char *cmd)
{
    void* L = pkcl::LuaStateManager::getInstance()->GetLuaState();
    if (!L) {
        dbg_log.push_back("Lua state is unavailable.");
        return;
    }
    const int initialTop = LuaH::getTop(L);
    struct StackGuard {
        void* state;
        int top;
        ~StackGuard() { LuaH::setTop(state, top); }
    } guard{L, initialTop};

    int ret = LuaH::loadBuffer(L, cmd, strlen(cmd), "poke-client VM", NULL);
    if (ret == LUA_OK) ret = LuaH::pcallk(L, 0, LUA_MULTRET, 0, 0, 0);
    if(ret == LUA_OK){
        int nresults = LuaH::getTop(L) - initialTop;
        dbg_log.push_back(std::string("Returned ") + std::to_string(nresults) + std::string(" results."));
        for(int i = 0, j = initialTop + 1; i < nresults; i++, j++){
            int retType = LuaH::ltype(L, j);
            std::string res = pkcl::LuaStateManager::GetReturnTypeStr(retType) + std::string(": ");
            switch(retType){
                case LUA_TBOOLEAN:
                case LUA_TNUMBER:
                case LUA_TSTRING:
                case LUA_TTABLE:
                {
                    const char* text = LuaH::toString(L, j, NULL);
                    res += text ? text : "<value>";
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

    }
    else {
        const char* error = LuaH::toString(L, -1, NULL);
        dbg_log.push_back(std::string("Error in lua:\n") + (error ? error : "<unknown error>"));
    }
}

void LuaForm::ExecuteFile(std::string file)
{
    std::vector<char> buffer;
    {
        nn::fs::FileHandle handle;
        if (nn::fs::OpenFile(&handle, file.c_str(), nn::fs::OpenMode_Read) != 0) {
            dbg_log.push_back("Failed to open Lua script.");
            return;
        }
        struct FileGuard {
            nn::fs::FileHandle handle;
            ~FileGuard() { nn::fs::CloseFile(handle); }
        } guard{handle};
        long size = 0;
        if (nn::fs::GetFileSize(&size, handle) != 0 || size < 0 ||
            static_cast<uint64_t>(size) >= buffer.max_size()) {
            dbg_log.push_back("Failed to read Lua script size.");
            return;
        }
        buffer.resize(static_cast<size_t>(size) + 1, '\0');
        if (nn::fs::ReadFile(handle, 0, buffer.data(), size) != 0) {
            dbg_log.push_back("Failed to read Lua script.");
            return;
        }
    }
    ExecuteCmd(buffer.data());
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
        ImGui::TextUnformatted(l.c_str());
        
	ImGui::EndChild();
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::Text("Debug");
    
    ImGui::BeginChild("##scrolling_dbg", ImVec2(0, -bottom_elem), ImGuiChildFlags_Border);
	for(auto &l : dbg_log)
        ImGui::TextUnformatted(l.c_str());
        
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
        if(!scriptList.empty())
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