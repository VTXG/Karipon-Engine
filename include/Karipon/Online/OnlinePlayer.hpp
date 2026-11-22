#pragma once

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
        char mStageName[32];
        s32 mScenarioNo;
        TVec3f mPosition;
        TVec3f mRotation;
        TVec3f mScale;
        char mAnimName[32];
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
