#include "filelogger.hpp"

pkcl::FileLogger *pkcl::FileLogger::instance = nullptr;

void pkcl::FileLogger::Log(std::string str)
{
    WriteFile(str.c_str(), str.size());
}

void pkcl::FileLogger::Log(std::deque<std::string> logs)
{
    std::string outStr = "";
    for(auto s : logs){
        outStr += (s + '\n');
        
    }
    WriteFile(outStr.c_str(), outStr.size());
}

void pkcl::FileLogger::LogRaw(char *str, size_t size)
{
    WriteFile(str, size, true);
}

void pkcl::FileLogger::WriteFile(const char *str, size_t size, bool isBin)
{
    pkcl::FilesystemManager::getInstance()->TryMountSD();
    nn::fs::DirectoryEntryType type;
    std::string filename = "sd:/" + logName + (isBin ? ".bin" : ".txt");
    Result rc = nn::fs::GetEntryType(&type, filename.c_str());
    if (rc == 0x202)
        rc = nn::fs::CreateFile(filename.c_str(), 0);
    
    s64 offset = 0;
    nn::fs::OpenFile(&handleOut, filename.c_str(), nn::fs::OpenMode_Write | nn::fs::OpenMode_Append);
    str += '\n';
    nn::fs::WriteFile(handleOut, offset, str, size, nn::fs::WriteOption::MakeValue(nn::fs::WriteOptionFlag_Flush));
    nn::fs::CloseFile(handleOut);
}