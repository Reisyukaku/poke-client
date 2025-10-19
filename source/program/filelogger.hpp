#pragma once

#include <stdarg.h>
#include <deque>
#include <string>
#include <nn/fs.hpp>
#include "filesystemManager.hpp"
#include <nn/os.hpp>

namespace pkcl {
class FileLogger {
public:
	void Log(const char* fmt, ...);
    void Log(std::string str);
    void Log(std::deque<std::string> logs);
	void LogRaw(const char *str, size_t size);

	static FileLogger* getInstance() {
		if (instance == nullptr)
			instance = new FileLogger();
		return instance;
	};

	FileLogger(const FileLogger&) = delete;
	FileLogger& operator=(const FileLogger&) = delete;

private:
	static FileLogger* instance;
	nn::os::MutexType m_mutex;
	
    nn::fs::FileHandle handleOut;
    std::string logName;
	bool mounted = false;
	void WriteFile(const char *str, size_t size, bool isBin = false);
    
	FileLogger();
	~FileLogger();
};
}