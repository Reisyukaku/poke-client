#pragma once

#include <string>
#include <cstring>
#include <unistd.h>
#include <map>
#include "../types.h"
#include "nn/socket.hpp"
#include "nn/result.h"
#include "nn/util.h"
#include <stdarg.h>

#define PACKET_MAX_SIZE 0x500

enum class SocketState {
    UNINITIALIZED = 0,
    CONNECTED = 1,
    UNAVAILABLE = 2,
    DISCONNECTED = 3
};

namespace pkcl {
class TcpLogger {
public:
	static TcpLogger* getInstance() {
		if (instance == nullptr)
			instance = new TcpLogger();
		return instance;
	};
    
    nn::Result init(const char* ip, u16 port);

    static void PrintString(const char* fmt, ...);
    static void PrintHex(char *buffer, size_t size);
    static const char* receiveMessage();
    void close();
    int GetDesc();
    bool IsConnected(){
        return mState == SocketState::CONNECTED;
    }
    
protected:
    SocketState mState;

private:
	static TcpLogger* instance;
    int mSocketFd;
    
	TcpLogger() {
		//
	}
	
	TcpLogger(const TcpLogger&);
	TcpLogger& operator=(const TcpLogger&);

};
};