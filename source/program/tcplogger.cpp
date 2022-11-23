#include "tcplogger.hpp"
#include "nn/nifm.h"
#include "nn/socket.hpp"

constexpr inline auto DefaultTcpAutoBufferSizeMax      = 192 * 1024;
constexpr inline auto MinTransferMemorySize            = (2 * DefaultTcpAutoBufferSizeMax + (128 * 1024));
constexpr inline auto MinSocketAllocatorSize           = 128 * 1024;
constexpr inline auto SocketAllocatorSize = MinSocketAllocatorSize * 1;
constexpr inline auto TransferMemorySize = MinTransferMemorySize * 1;
constexpr inline auto SocketPoolSize = SocketAllocatorSize + TransferMemorySize;
char socketPool[SocketPoolSize] __attribute__((aligned(0x4000)));
exl::TcpLogger *exl::TcpLogger::instance = nullptr;

nn::Result exl::TcpLogger::init(const char *ip, u16 port) {
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

void exl::TcpLogger::close() {
    mState = SocketState::UNINITIALIZED;
}

void exl::TcpLogger::sendMessage(const char *message) {
    if (mState != SocketState::CONNECTED)
        return;

    size_t length = strlen(message);
    size_t offset = 0;
    while (offset < length) {
        size_t minSize = std::min(length - offset, (size_t)PACKET_MAX_SIZE);
        int sent = nn::socket::Send(mSocketFd, message + offset, minSize, 0);
        offset += sent;
    }
}

const char *exl::TcpLogger::receiveMessage() {
    if (mState != SocketState::CONNECTED)
        return nullptr;

    std::string message;
    char buffer[PACKET_MAX_SIZE] = {};
    while (!message.ends_with('\n')) {
        if (nn::socket::Recv(mSocketFd, buffer, sizeof(buffer), 0) > 0) {
            message.append(buffer);
        } else {
            return nullptr;
        }
    }

    char* ptr = (char*)malloc(message.length() * sizeof(char));
    strcpy(ptr, message.c_str());

    return ptr;
}
