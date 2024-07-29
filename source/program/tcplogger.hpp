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
#include "offsetManager.hpp"

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
    static const char* receiveMessage();
    void close();
    static ssize_t stdio_write(struct _reent* r, void *fd, const char* ptr, size_t len);
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
    SocketState mState;

private:
	static TcpLogger* instance;
    int mSocketFd;
    
	TcpLogger() {}
    ~TcpLogger() {}
};
};