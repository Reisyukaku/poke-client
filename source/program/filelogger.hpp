#pragma once

#include <stdarg.h>
#include <string>
#include <deque>
#include "nn/fs.hpp"

namespace exl {
class FileLogger {
public:
	static FileLogger* getInstance() {
		if (instance == nullptr)
			instance = new FileLogger();
		return instance;
	};
    
    void Log(std::string str){
        std::string path("sd:/logs.txt");
        nn::fs::DirectoryEntryType type;
        Result rc = nn::fs::GetEntryType(&type, path.c_str());
        if (rc == 0x202)
            rc = nn::fs::CreateFile(path.c_str(), 0);
        
        s64 offset = 0;
        nn::fs::OpenFile(&handleOut, path.c_str(), nn::fs::OpenMode_Write | nn::fs::OpenMode_Append);
        str += '\n';
        nn::fs::WriteFile(handleOut, offset, str.c_str(), str.size(), nn::fs::WriteOption::MakeValue(nn::fs::WriteOptionFlag_Flush));
        nn::fs::CloseFile(handleOut);
    }
    
    void Log(std::deque<std::string> logs){
        std::string path("sd:/logs.txt");
        nn::fs::DirectoryEntryType type;
        Result rc = nn::fs::GetEntryType(&type, path.c_str());
        if (rc == 0x202)
            rc = nn::fs::CreateFile(path.c_str(), 0);
        
        s64 offset = 0;
        nn::fs::OpenFile(&handleOut, path.c_str(), nn::fs::OpenMode_Write | nn::fs::OpenMode_Append);
        for(auto s : logs){
            s += '\n';
            nn::fs::WriteFile(handleOut, offset, s.c_str(), s.size(), nn::fs::WriteOption::MakeValue(nn::fs::WriteOptionFlag_Flush));
            offset += s.size();
        }
        nn::fs::CloseFile(handleOut);
    }
    
	
private:
    nn::fs::FileHandle handleOut;
    std::string logPath;
    
	FileLogger() {
		nn::fs::MountSdCardForDebug("sd");
	}
    
    static FileLogger* instance;
	FileLogger(const FileLogger&);
	FileLogger& operator=(const FileLogger&);
    
};
};