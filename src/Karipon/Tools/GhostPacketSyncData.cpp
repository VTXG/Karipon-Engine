#include "Karipon/Tools/GhostPacketSyncData.hpp"
#include <Game/Animation/XanimePlayer.hpp>
#include <Game/LiveActor/ModelManager.hpp>
#include <Game/Player/MarioAccess.hpp>
#include <Game/Player/MarioActor.hpp>
#include <Game/Util/HashUtil.hpp>
#include <Game/Util/MathUtil.hpp>
#include <Game/Util/MtxUtil.hpp>

namespace {
    // 8037F4B0
    void normalizeAngle(f32* pAngle) {
        *pAngle = MR::repeat(*pAngle, 0.0f, TWO_PI);

        if (*pAngle > PI) {
            *pAngle -= TWO_PI;
        }

        *pAngle *= 1.0f / PI;
    }

    // 8037F530
    f32 getShiftRatio(s8 shiftValue) {
        if (shiftValue > 0) {
            return static_cast< f32 >(256 << shiftValue) / 256.0f;
        }
        return static_cast< f32 >(256 >> -shiftValue) / 256.0f;
    }

    // 8037F5B0
    // also checks if the conversion overflowed
    bool packFloat(const TVec3f& rPosition, s8 shift, TVec3s* pOut) {
        f32 ratio = getShiftRatio(shift);
        s32 x = static_cast< s32 >(rPosition.x * ratio);
        s32 y = static_cast< s32 >(rPosition.y * ratio);
        s32 z = static_cast< s32 >(rPosition.z * ratio);
        pOut->set(x, y, z);

        if (pOut->x != x || pOut->y != y || pOut->z != z) {
            return true;
        }

        return false;
    }

    // 8037F680
    bool packFloat(f32 value, s8 shift, s16* pOut) {
        f32 ratio = getShiftRatio(shift);
        s32 v = static_cast< s32 >(value * ratio);
        *pOut = v;
        return *pOut != v;
    }

    // 8037F6F0
    // also checks if the conversion overflowed
    bool packFloat(const TVec3f& rValue, s8 shift, TVec3Sc* pOut) {
        f32 ratio = getShiftRatio(shift);
        s32 x = static_cast< s32 >(rValue.x * ratio);
        s32 y = static_cast< s32 >(rValue.y * ratio);
        s32 z = static_cast< s32 >(rValue.z * ratio);
        pOut->set(x, y, z);

        if (pOut->x != x || pOut->y != y || pOut->z != z) {
            return true;
        }

        return false;
    }

    // 8037F7D0
    bool packFloat(f32 value, s8 shift, s8* pOut) {
        f32 ratio = getShiftRatio(shift);
        s32 v = static_cast< s32 >(value * ratio);
        *pOut = v;
        return *pOut != v;
    }

    // 8037FA30
    bool packPosition(TVec3s* pOut, MtxPtr pMtx) {
        TVec3f position;
        MR::extractMtxTrans(pMtx, &position);
        return packFloat(position, -2, pOut);
    }

    // 8037FA80
    bool packRotation(TVec3Sc* pOut, const TVec3f& rRotation) {
        TVec3f rotation(rRotation);
        normalizeAngle(&rotation.x);
        normalizeAngle(&rotation.y);
        normalizeAngle(&rotation.z);
        return packFloat(rotation, 7, pOut);
    }

    // 8037FAF0
    bool packRotation(TVec3Sc* pOut, MtxPtr pMtx) {
        TVec3f rotation;
        TPos3f mtx(pMtx);
        mtx.getEulerXYZ(rotation);
        return packRotation(pOut, rotation);
    }

    // 8037FC30
    bool packScale(TVec3Sc* pOut, const TVec3f& rScale) {
        return packFloat(rScale, 3, pOut);
    }

    // 8037FC40
    bool packVelocity(TVec3Sc* pOut, const TVec3f& rVelocity) {
        return packFloat(rVelocity, 0, pOut);
    }

    // 8037FC50
    bool packAnimFrame(s16* pOut, f32 frame) {
        return packFloat(frame, 5, pOut);
    }

    // 8037FC60
    bool packTrackWeight(s8* pOut, f32 frame) {
        return packFloat(frame, 7, pOut);
    }

    // 8037FC70
    bool packAnimRate(s8* pOut, f32 frame) {
        return packFloat(frame, 3, pOut);
    }
}; // namespace

namespace gst {
    GhostPacketSyncData::GhostPacketSyncData()
        : mPosition(0.0f), mPositionPacked(0, 0, 0), mRotation(0), mScale(0), mVelocity(0), mAnimName(), mAnimHash(), mUseBckHash(),
          mUsePositionPacked(), mAnimFrame(), mAnimRate() {
        for (s32 i = 0; i < ARRAY_SIZE(mTrackWeights); i++) {
            mTrackWeights[i] = 0;
        }
    }

    void GhostPacketSyncData::syncWithPlayer() {
        syncWithActor(MarioAccess::getPlayerActor(), true);
    }

    void GhostPacketSyncData::syncWithActor(const LiveActor* pActor, bool syncAnims) {
        XanimePlayer* animePlayer = pActor->mModelManager->mXanimePlayer;
        MR::extractMtxTrans(pActor->getBaseMtx(), &mPosition);

        if (packPosition(&mPositionPacked, pActor->getBaseMtx())) {
            mUsePositionPacked = false;
        } else {
            mUsePositionPacked = true;
        }

        packRotation(&mRotation, pActor->getBaseMtx());
        packScale(&mScale, pActor->mScale);
        packVelocity(&mVelocity, pActor->mVelocity);

        if (syncAnims) {
            if (animePlayer->isAnimationRunSimple()) {
                mUseBckHash = false;
                mAnimName = animePlayer->getCurrentBckName();
                mAnimHash = 0;
            } else {
                mUseBckHash = true;
                mAnimName = animePlayer->getCurrentAnimationName();
                mAnimHash = MR::getHashCode(mAnimName);
            }
        }

        packAnimFrame(&mAnimFrame, animePlayer->tellAnimationFrame());
        packAnimRate(&mAnimRate, animePlayer->getRate());

        for (s32 i = 0; i < ARRAY_SIZE(mTrackWeights); i++) {
            packTrackWeight(&mTrackWeights[i], animePlayer->mWeights[i]);
        }
    }
}; // namespace gst
