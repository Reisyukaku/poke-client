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

	FileLogger(const FileLogger&) = delete;
	FileLogger& operator=(const FileLogger&) = delete;

private:
	static FileLogger* instance;
	
    nn::fs::FileHandle handleOut;
    std::string logPath;
    
	FileLogger() : logPath("sd:/logs.txt") {
		//
	}    
};
}