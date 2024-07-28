#pragma once

#include <stdarg.h>
#include <deque>
#include <string>
#include <nn/fs.hpp>
#include "filesystemManager.hpp"

namespace pkcl {
class FileLogger {
public:
	void init();
    void Log(std::string str);
    void Log(std::deque<std::string> logs);
	void LogRaw(char *str, size_t size);
    
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
    std::string logName;
	bool mounted = false;
	void WriteFile(const char *str, size_t size, bool isBin = false);
    
	FileLogger() : logName("logs") {
		//
	}    
};
}