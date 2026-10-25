#pragma once

#include <string>
#include <optional>
#include <cstring>
#include <unistd.h>
#include <map>
#include "../types.h"
#include "nn/socket.hpp"
#include "nn/result.h"
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
    nn::Result init(const char* ip, u16 port);

    static void SendMessage(const char* msg);
    static std::optional<std::string> receiveMessage();
    void close();
    
    bool IsConnected(){
        return mState == SocketState::CONNECTED;
    }

    static TcpLogger* getInstance() {
		if (instance == nullptr)
			instance = new TcpLogger();
		return instance;
	};

    TcpLogger(const TcpLogger&) = delete;
	TcpLogger& operator=(const TcpLogger&) = delete;
    
protected:
    SocketState mState = SocketState::UNINITIALIZED;

private:
	static TcpLogger* instance;
    int mSocketFd = -1;
    bool networkInitialized = false;
    bool socketsInitialized = false;
    
	TcpLogger() {}
    ~TcpLogger() { close(); }
};
};