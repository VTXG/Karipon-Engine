#pragma once

#include "Karipon/Online/OnlinePacket.hpp"
#include <Game/LiveActor/LiveActor.hpp>

class XanimePlayer;

namespace krp {
    class OnlinePlayerActor;

    class OnlinePlayerState {
    public:
        OnlinePlayerState();

        void createActor();
        void destroyActor();
        void reset();

        OnlinePlayerActor* mActor;
        OnlineString< 32 > mStageName;
        s32 mScenarioNo;
        TVec3f mPosition;
        TVec3f mRotation;
        TVec3f mScale;
        OnlineString< 32 > mAnimName;
        u32 mAnimNameHash;
        f32 mAnimFrame;
        f32 mAnimWeights[4];
        bool mIsSimpleAnimation;
    };

    class OnlinePlayerActor : public LiveActor {
    public:
        OnlinePlayerActor(OnlinePlayerState* pState);

        virtual void init(const JMapInfoIter& rIter);

        OnlinePlayerState* mState;
        XanimePlayer* mXanimePlayer;
        XanimePlayer* mXanimePlayerUpper;
    };
}; // namespace krp
