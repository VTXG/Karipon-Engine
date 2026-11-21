#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class LiveActor;

namespace gst {
    class GhostPacketSyncData {
    public:
        GhostPacketSyncData();

        void syncWithPlayer();
        void syncWithActor(const LiveActor* pActor, bool syncAnims);

        /* 0x00 */ TVec3f mPosition;
        /* 0x0C */ TVec3s mPositionPacked;
        /* 0x12 */ TVec3Sc mRotation;
        /* 0x15 */ TVec3Sc mScale;
        /* 0x18 */ TVec3Sc mVelocity;
        /* 0x1C */ const char* mAnimName;
        /* 0x20 */ u32 mAnimHash;
        /* 0x24 */ bool mUseBckHash;
        /* 0x25 */ bool mUsePositionPacked;
        /* 0x26 */ s16 mAnimFrame;
        /* 0x28 */ s8 mTrackWeights[4];
        /* 0x2C */ s8 mAnimRate;
    };
}; // namespace gst
