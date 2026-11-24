#include "Karipon/Online/OnlineManager.hpp"
#include "Karipon/Online/OnlinePacket.hpp"
#include <Game/LiveActor/Nerve.hpp>
#include <Game/Util/MemoryUtil.hpp>
#include <Game/Util/NerveUtil.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>

namespace {
    NEW_NERVE(OnlineManagerNrvDisconnected, krp::OnlineManager, Disconnected);
    NEW_NERVE(OnlineManagerNrvConnecting, krp::OnlineManager, Connecting);
    NEW_NERVE(OnlineManagerNrvConnected, krp::OnlineManager, Connected);
}; // namespace

namespace krp {
    OnlineManager::OnlineManager() : NerveExecutor("OnlineManager"), mSocket(-1), mServerAddress(), mPacketBat(PacketCount) {
        mPacketArena = new (MR::getStationedHeapNapa(), PacketAlign) u8[PacketBufferSize * PacketCount];
        mRecvBuffer = allocPacket();
        mSendBuffer = allocPacket();

        initNerve(GET_NERVE_ANON(OnlineManagerNrvDisconnected));
    }

    void OnlineManager::init() {
        if (isNerve(GET_NERVE_ANON(OnlineManagerNrvDisconnected))) {
            setNerve(GET_NERVE_ANON(OnlineManagerNrvConnecting));
        }
    }

    void OnlineManager::close() {
        if (!isNerve(GET_NERVE_ANON(OnlineManagerNrvDisconnected))) {
            krp::NetworkFunction::close();
            setNerve(GET_NERVE_ANON(OnlineManagerNrvDisconnected));
        }
    }

    void OnlineManager::update() {
        updateNerve();
    }

    u8* OnlineManager::allocPacket() {
        s32 i = 0;
        for (; i < mPacketBat.size(); i++) {
            if (!mPacketBat.isOn(i)) {
                break;
            }
        }

        if (i == mPacketBat.size()) {
            return nullptr;
        }

        mPacketBat.set(i, true);
        return mPacketArena + PacketBufferSize;
    }

    void OnlineManager::freePacket(u8* pData) {
        s32 i = (mPacketArena - pData) / PacketBufferSize;
        if (i < 0 || i >= mPacketBat.size()) {
            return;
        }

        mPacketBat.set(i, false);
    }

    void OnlineManager::exeDisconnected() {}

    void OnlineManager::exeConnecting() {
        if (MR::isFirstStep(this)) {
            krp::NetworkFunction::init();
        }

        krp::NetworkState state = krp::NetworkFunction::getState();
        if (state == krp::NetworkState_Initializing) {
            return;
        }

        IOSError err = krp::NetworkFunction::getError();
        if (err >= 0) {
            mSocket = err = krp::NetworkFunction::initSocket(SO_AF_INET, SO_SOCK_DGRAM, 0);

            if (err >= 0) {
                setNerve(GET_NERVE_ANON(OnlineManagerNrvConnected));
                return;
            }
        }

        // TODO error handling
        close();
    }

    void OnlineManager::exeConnected() {
        for (u32 i = 0; i < 20; i++) {
            IOSError err = krp::NetworkFunction::recv(mSocket, mRecvBuffer, PacketBufferSize, SO_MSG_NONBLOCK);

            if (err < 0) {
                break;
            }

            krp::OnlinePacketStream stream(mRecvBuffer, err, PacketBufferSize);
        }
    }
}; // namespace krp
