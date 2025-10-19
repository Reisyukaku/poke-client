#include "filelogger.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>

pkcl::FileLogger *pkcl::FileLogger::instance = nullptr;

pkcl::FileLogger::FileLogger() : logName("logs") 
{
    nn::os::InitializeMutex(&m_mutex, false, 0);
}

pkcl::FileLogger::~FileLogger()
{
    nn::os::FinalizeMutex(&m_mutex);
}

void pkcl::FileLogger::Log(const char* fmt, ...)
{
    char buffer[512];  // adjust size as needed

    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    // Clamp to avoid writing beyond buffer
    if (len < 0)
        return;

    if (len >= static_cast<int>(sizeof(buffer)))
        len = sizeof(buffer) - 1;

    WriteFile(buffer, len);
}

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

void pkcl::FileLogger::LogRaw(const char *str, size_t size)
{
    WriteFile(str, size, true);
}

void pkcl::FileLogger::WriteFile(const char *str, size_t size, bool isBin)
{
    nn::os::LockMutex(&m_mutex);

    pkcl::FilesystemManager::getInstance()->TryMountSD();
    nn::fs::DirectoryEntryType type;
    std::string filename = "sd:/" + logName + (isBin ? ".bin" : ".txt");
    Result rc = nn::fs::GetEntryType(&type, filename.c_str());
    if (rc == 0x202)
        rc = nn::fs::CreateFile(filename.c_str(), 0);
    
    s64 offset = 0;
    nn::fs::OpenFile(&handleOut, filename.c_str(), nn::fs::OpenMode_Write | nn::fs::OpenMode_Append);

    s64 fileSize = 0;
    nn::fs::GetFileSize(&fileSize, handleOut);

    nn::fs::WriteFile(handleOut, fileSize, str, size, nn::fs::WriteOption::MakeValue(nn::fs::WriteOptionFlag_Flush));
    nn::fs::CloseFile(handleOut);

    nn::os::UnlockMutex(&m_mutex);
}