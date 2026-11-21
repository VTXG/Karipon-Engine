#pragma once

#include "Karipon/System/NetworkFunction.hpp"
#include <Game/System/NerveExecutor.hpp>

namespace krp {
    class OnlineManager : public NerveExecutor {
    public:
        OnlineManager();

        void init();
        void close();
        void update();

        void exeDisconnected();
        void exeConnecting();
        void exeConnected();

        IOSFd mSocket;
        SockAddress mServerAddress;
    };
}; // namespace krp
