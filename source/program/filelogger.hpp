#pragma once

#include <stdarg.h>
#include <deque>
#include <string>
#include <nn/fs.hpp>

namespace pkcl {
class FileLogger {
public:
	void init();
    void Log(std::string str);
    void Log(std::deque<std::string> logs);
    
	static FileLogger* getInstance() {
		if (instance == nullptr)
			instance = new FileLogger();
		return instance;
	};

private:
    nn::fs::FileHandle handleOut;
    std::string logPath;
    
    static FileLogger* instance;
	FileLogger(const FileLogger&);
	FileLogger& operator=(const FileLogger&);
    
	FileLogger() : logPath("sd:/logs.txt") {
		//
	}    
};
}