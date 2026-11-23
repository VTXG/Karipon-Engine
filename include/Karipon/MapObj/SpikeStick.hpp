#pragma once

#include <Game/LiveActor/LiveActor.hpp>

namespace krp {
    class SpikeStick : public LiveActor {
    public:
        enum MovementEasing {
            MovementEasing_None,
            MovementEasing_In,
            MovementEasing_InOut,
        };

        SpikeStick(const char* pName);

        virtual void init(const JMapInfoIter& rIter);
        virtual void calcAndSetBaseMtx();

        void updateMove(f32 maxStep, f32 from, f32 to, MovementEasing easing);

        void exeWait();
        void exeMoveStart();
        void exeMove();
        void exeMoveEnd();
        void exeMoveBack();

        TVec3f mUp;
        TVec3f mFront;
        TVec3f mHomePosition;
        TVec3f mStopPosition;
        f32 mMoveCoord;

        s32 mStepDelay;
        s32 mStepWait;
        s32 mStepMove;
        s32 mStepMoveWait;
        s32 mStepMoveBack;
    };
};
