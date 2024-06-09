#include "filelogger.hpp"

pkcl::FileLogger *pkcl::FileLogger::instance = nullptr;

void pkcl::FileLogger::init()
{
    nn::fs::MountSdCardForDebug("sd");
}

void pkcl::FileLogger::Log(std::string str)
{
    nn::fs::DirectoryEntryType type;
    Result rc = nn::fs::GetEntryType(&type, logPath.c_str());
    if (rc == 0x202)
        rc = nn::fs::CreateFile(logPath.c_str(), 0);
    
    s64 offset = 0;
    nn::fs::OpenFile(&handleOut, logPath.c_str(), nn::fs::OpenMode_Write | nn::fs::OpenMode_Append);
    str += '\n';
    nn::fs::WriteFile(handleOut, offset, str.c_str(), str.size(), nn::fs::WriteOption::MakeValue(nn::fs::WriteOptionFlag_Flush));
    nn::fs::CloseFile(handleOut);
}

void pkcl::FileLogger::Log(std::deque<std::string> logs)
{
    nn::fs::DirectoryEntryType type;
    Result rc = nn::fs::GetEntryType(&type, logPath.c_str());
    if (rc == 0x202)
        rc = nn::fs::CreateFile(logPath.c_str(), 0);
    
    s64 offset = 0;
    nn::fs::OpenFile(&handleOut, logPath.c_str(), nn::fs::OpenMode_Write | nn::fs::OpenMode_Append);
    for(auto s : logs){
        s += '\n';
        nn::fs::WriteFile(handleOut, offset, s.c_str(), s.size(), nn::fs::WriteOption::MakeValue(nn::fs::WriteOptionFlag_Flush));
        offset += s.size();
    }
    nn::fs::CloseFile(handleOut);
}