#pragma once

#include "Karipon/System/NetworkFunction.hpp"
#include <Game/System/NerveExecutor.hpp>
#include <Game/Util/BitArray.hpp>

namespace krp {
    class OnlineManager : public NerveExecutor {
    public:
        static const int PacketAlign = 32;
        static const int PacketCount = 32;
        static const int PacketBufferSize = ALIGN_NEXT(0x100, PacketAlign);

        OnlineManager();

        void init();
        void close();
        void update();

        u8* allocPacket();
        void freePacket(u8* pData);

        void exeDisconnected();
        void exeConnecting();
        void exeConnected();

        IOSFd mSocket;
        SockAddress mServerAddress;

        MR::BitArray mPacketBat;
        u8* mPacketArena;
        u8* mRecvBuffer;
        u8* mSendBuffer;
    };
}; // namespace krp
