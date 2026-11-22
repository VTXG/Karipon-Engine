#include "Karipon/Online/OnlineManager.hpp"
#include <Game/LiveActor/Nerve.hpp>
#include <Game/Util/NerveUtil.hpp>

namespace {
    NEW_NERVE(OnlineManagerNrvDisconnected, krp::OnlineManager, Disconnected);
    NEW_NERVE(OnlineManagerNrvConnecting, krp::OnlineManager, Connecting);
    NEW_NERVE(OnlineManagerNrvConnected, krp::OnlineManager, Connected);
}; // namespace

namespace krp {
    OnlineManager::OnlineManager() : NerveExecutor("OnlineManager"), mSocket(-1), mServerAddress() {
        initNerve(GET_NERVE_ANON(OnlineManagerNrvDisconnected));
    }

    void OnlineManager::init() {}

    void OnlineManager::close() {}

    void OnlineManager::update() {
        updateNerve();
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
        krp::NetworkFunction::close();
        setNerve(GET_NERVE_ANON(OnlineManagerNrvDisconnected));
    }

    void OnlineManager::exeConnected() {
        for (u32 i = 0; i < 50; i++) {
            char buffer[0x100];
            IOSError err = krp::NetworkFunction::recv(mSocket, buffer, sizeof(buffer), SO_MSG_NONBLOCK);

            if (err < 0) {
                break;
            }
        }
    }
}; // namespace krp
