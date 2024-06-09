#include "tcplogger.hpp"
#include "nn/nifm.h"
#include "nn/socket.hpp"

constexpr inline auto DefaultTcpAutoBufferSizeMax      = 192 * 1024; /* 192kb */
constexpr inline auto MinTransferMemorySize            = (2 * DefaultTcpAutoBufferSizeMax + (128 * 1024));
constexpr inline auto MinSocketAllocatorSize           = 128 * 1024;

constexpr inline auto SocketAllocatorSize = MinSocketAllocatorSize * 1;
constexpr inline auto TransferMemorySize = MinTransferMemorySize * 1;

constexpr inline auto SocketPoolSize = SocketAllocatorSize + TransferMemorySize;

char socketPool[SocketPoolSize] __attribute__((aligned(0x4000)));

pkcl::TcpLogger *pkcl::TcpLogger::instance = nullptr;

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

    return result;
}

void pkcl::TcpLogger::close() {
    mState = SocketState::UNINITIALIZED;
}

void pkcl::TcpLogger::PrintHex(char *buffer, size_t size) {
    char b[4] = {};
    int i = 0;
    while(i++ < size){
        nn::util::SNPrintf(b, 4, "%02X%s", buffer[i], (i % 16 == 0 && i > 0) ? "\n" : " ");
        nn::socket::Send(getInstance()->mSocketFd, b, 4, 0);
    }
}

void pkcl::TcpLogger::PrintString(const char *fmt, ...) {
    if (getInstance()->mState != SocketState::CONNECTED)
        return;

    va_list args;
    va_start(args, fmt);
    char buffer[0x1000] = {};

    nn::util::VSNPrintf(buffer, sizeof(buffer), fmt, args);

    nn::socket::Send(getInstance()->mSocketFd, buffer, sizeof(buffer), 0);
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
