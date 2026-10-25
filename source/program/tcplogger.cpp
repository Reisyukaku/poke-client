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
    in_addr hostAddress{};
    sockaddr serverAddress{};

    if (mState != SocketState::UNINITIALIZED)
        return -1;

    if (!networkInitialized) {
        auto result = nn::nifm::Initialize();
        if (result.isFailure()) return result;
        networkInitialized = true;
    }

    nn::nifm::SubmitNetworkRequest();

    while (nn::nifm::IsNetworkRequestOnHold()) { }

    if (!nn::nifm::IsNetworkAvailable()) {
        mState = SocketState::UNAVAILABLE;
        return -1;
    }

    nn::socket::Config config{};

    config.pool = socketPool;
    config.allocPoolSize = SocketAllocatorSize;
    config.poolSize = SocketPoolSize;
    config.concurLimit = 0xE;

    if (!socketsInitialized) {
        auto result = nn::socket::Initialize(config);
        if (result.isFailure()) return result;
        socketsInitialized = true;
    }

    if ((mSocketFd = nn::socket::Socket(2, 1, 0)) < 0) {
        mState = SocketState::UNAVAILABLE;
        return -1;
    }

    nn::socket::InetAton(ip, &hostAddress);

    serverAddress.address = hostAddress;
    serverAddress.port = nn::socket::InetHtons(port);
    serverAddress.family = 2;

    nn::Result result = nn::socket::Connect(mSocketFd, &serverAddress, sizeof(serverAddress));

    if (result.isSuccess()) {
        mState = SocketState::CONNECTED;
    } else {
        close();
        mState = SocketState::DISCONNECTED;
    }
    
    return result;
}

void pkcl::TcpLogger::close() {
    if (mSocketFd >= 0) {
        nn::socket::Close(mSocketFd);
        mSocketFd = -1;
    }
    mState = SocketState::UNINITIALIZED;
}

void pkcl::TcpLogger::SendMessage(const char *msg) {
    if (getInstance()->mState != SocketState::CONNECTED)
        return;

    nn::socket::Send(getInstance()->mSocketFd, msg, strlen(msg), 0);
}

std::optional<std::string> pkcl::TcpLogger::receiveMessage() {
    auto* logger = getInstance();
    if (logger->mState != SocketState::CONNECTED) return std::nullopt;

    constexpr size_t MaxMessageSize = 64 * 1024;
    std::string message;
    char buffer[PACKET_MAX_SIZE];
    while (message.find('\n') == std::string::npos) {
        const auto received = nn::socket::Recv(logger->mSocketFd, buffer, sizeof(buffer), 0);
        if (received <= 0 || static_cast<size_t>(received) > MaxMessageSize - message.size()) {
            logger->close();
            logger->mState = SocketState::DISCONNECTED;
            return std::nullopt;
        }
        message.append(buffer, static_cast<size_t>(received));
    }
    return message;
}
