#include "tcplogger.hpp"
#include "nn/nifm.h"
#include "nn/socket.hpp"
#include <dirent.h>

constexpr inline auto DefaultTcpAutoBufferSizeMax      = 192 * 1024; /* 192kb */
constexpr inline auto MinTransferMemorySize            = (2 * DefaultTcpAutoBufferSizeMax + (128 * 1024));
constexpr inline auto MinSocketAllocatorSize           = 128 * 1024;

constexpr inline auto SocketAllocatorSize = MinSocketAllocatorSize * 1;
constexpr inline auto TransferMemorySize = MinTransferMemorySize * 1;

constexpr inline auto SocketPoolSize = SocketAllocatorSize + TransferMemorySize;

char socketPool[SocketPoolSize] __attribute__((aligned(0x4000)));

pkcl::TcpLogger *pkcl::TcpLogger::instance = nullptr;

static const devoptab_t std_devop = {
    "console",
    0,
    NULL,
    NULL,
    pkcl::TcpLogger::stdio_write,
    NULL,
    NULL,
    NULL
};

ssize_t pkcl::TcpLogger::stdio_write(struct _reent* r, void *fd, const char* ptr, size_t len) {
    getInstance()->SendMessage(ptr);
    return len;
}

nn::Result pkcl::TcpLogger::init(const char *ip, u16 port) {
    in_addr hostAddress = { 0 };
    sockaddr serverAddress = { 0 };

    if (mState != SocketState::UNINITIALIZED)
        return -1;

    nn::nifm::Initialize();

    nn::nifm::SubmitNetworkRequest();

    while (nn::nifm::IsNetworkRequestOnHold()) { }

    if (!nn::nifm::IsNetworkAvailable()) {
        mState = SocketState::UNAVAILABLE;
        return -1;
    }

    nn::socket::Config config;

    config.pool = socketPool;
    config.allocPoolSize = SocketAllocatorSize;
    config.poolSize = SocketPoolSize;
    config.concurLimit = 0xE;

    nn::socket::Initialize(config);

    if ((mSocketFd = nn::socket::Socket(2, 1, 0)) < 0) {
        mState = SocketState::UNAVAILABLE;
        return -1;
    }

    nn::socket::InetAton(ip, &hostAddress);

    serverAddress.address = hostAddress;
    serverAddress.port = nn::socket::InetHtons(port);
    serverAddress.family = 2;

    nn::Result result = nn::socket::Connect(mSocketFd, &serverAddress, sizeof(serverAddress));

    mState = result.isSuccess() ? SocketState::CONNECTED : SocketState::DISCONNECTED;

    devoptab_list[ STD_OUT ] = &std_devop;
    setvbuf(stdout, NULL, _IONBF, 0);

    devoptab_list[ STD_ERR ] = &std_devop;
    setvbuf(stderr, NULL, _IONBF, 0);
    
    return result;
}

void pkcl::TcpLogger::close() {
    mState = SocketState::UNINITIALIZED;
}

void pkcl::TcpLogger::PrintHex(char *buffer, size_t size) {
    char buf[(size * 3) + 2] = {0};
    const char *hex = "0123456789ABCDEF";
    u8 i = 0, j = 0;
    while(i < size){
        u8 num1 = buffer[i] & 0xF;
        u8 num2 = (buffer[i] >> 4) & 0xF;
        
        buf[j++] = hex[num2];
        buf[j++] = hex[num1];
        buf[j++] = (i++ % 16 == 15) ? '\n' : ' ';
    }
    buf[(size * 3) + 1] = '\n';

    nn::socket::Send(getInstance()->mSocketFd, buf, (size * 3) + 1, 0);
}

void pkcl::TcpLogger::SendMessage(const char *msg) {
    if (getInstance()->mState != SocketState::CONNECTED)
        return;

    nn::socket::Send(getInstance()->mSocketFd, msg, strlen(msg), 0);
}

const char *pkcl::TcpLogger::receiveMessage() {
    if (getInstance()->mState != SocketState::CONNECTED)
        return nullptr;

    std::string message;
    char buffer[PACKET_MAX_SIZE] = {};
    while (!message.ends_with('\n')) {
        if (nn::socket::Recv(getInstance()->mSocketFd, buffer, sizeof(buffer), 0) > 0) {
            message.append(buffer);
        } else {
            return nullptr;
        }
    }

    char* ptr = (char*)malloc(message.length() * sizeof(char));
    strcpy(ptr, message.c_str());

    return ptr;
}
