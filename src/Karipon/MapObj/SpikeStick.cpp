#include "Karipon/MapObj/SpikeStick.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

/*

didy ahh sounds

SE_OJ_LV_ROCK_MOVE
SE_OJ_ROCK_BOUND
SE_OJ_LAVA_FLOATER_ON
SE_OJ_DOSSUN_LAND
SE_OJ_LV_METAL_LAND_MOVE
SE_OJ_METAL_LAND_SWITCH

*/

namespace {
    const f32 sCoordMoveStart = 0.05f;
    const f32 sHomeDistance = 2000.0f;

    NEW_NERVE(NrvSpikeStickWait, krp::SpikeStick, Wait);
    NEW_NERVE(NrvSpikeStickMoveStart, krp::SpikeStick, MoveStart);
    NEW_NERVE(NrvSpikeStickMove, krp::SpikeStick, Move);
    NEW_NERVE(NrvSpikeStickMoveEnd, krp::SpikeStick, MoveEnd);
    NEW_NERVE(NrvSpikeStickMoveBack, krp::SpikeStick, MoveBack);
}; // namespace

namespace krp {
    SpikeStick::SpikeStick(const char* pName)
        : LiveActor(pName), mUp(0.0f), mFront(0.0f), mHomePosition(0.0f), mStopPosition(0.0f), mMoveCoord(), mStepDelay(), mStepWait(90), mStepMove(60),
          mStepMoveWait(60), mStepMoveBack(210) {}

    void SpikeStick::init(const JMapInfoIter& rIter) {
        MR::initDefaultPos(this, rIter);
        initModelManagerWithAnm("SpikeStick", nullptr, false);
        MR::connectToSceneNoSilhouettedMapObj(this);
        MR::invalidateClipping(this);

        MR::getJMapInfoArg0NoInit(rIter, &mStepDelay);
        MR::getJMapInfoArg1NoInit(rIter, &mStepWait);
        MR::getJMapInfoArg2NoInit(rIter, &mStepMove);
        MR::getJMapInfoArg3NoInit(rIter, &mStepMoveWait);
        MR::getJMapInfoArg4NoInit(rIter, &mStepMoveBack);

        initHitSensor(1);
        MR::initCollisionParts(this, "SpikeStick", MR::addBodyMessageSensorMapObj(this), nullptr);

        TPos3f mtx;
        MR::makeMtxTR(mtx, this);

        mtx.getYDir(mUp);
        mtx.getZDir(mFront);
        mHomePosition = mPosition - mUp * ::sHomeDistance;
        mStopPosition = mPosition;

        initSound(4, false);
        initNerve(GET_NERVE_ANON(NrvSpikeStickWait));
        makeActorAppeared();
    }

    void SpikeStick::calcAndSetBaseMtx() {
        TVec3f pos = mHomePosition + (mStopPosition - mHomePosition) * mMoveCoord;
        TPos3f mtx;
        MR::makeMtxUpFrontPos(&mtx, mUp, mFront, pos);
        MR::rotateMtxLocalY(mtx, mMoveCoord * PI * 2.0f);
        MR::setBaseTRMtx(this, mtx);
    }

    void SpikeStick::updateMove(f32 maxStep, f32 from, f32 to, MovementEasing easing) {
        f32 step;
        bool isReverse;

        if (from < to) {
            step = static_cast< f32 >(getNerveStep());
            isReverse = false;
        } else {
            step = maxStep - static_cast< f32 >(getNerveStep());
            f32 tmp = to;
            to = from;
            from = tmp;
            isReverse = true;
        }

        f32 coord = MR::getInterpolateValue(step / maxStep, from, to);

        switch (easing) {
        case MovementEasing_In:
            coord = MR::getEaseInValue(coord - from, from, to, to - from);
            break;
        case MovementEasing_InOut:
            coord = MR::getEaseInOutValue(coord - from, from, to, to - from);
            break;
        default:
            break;
        }

        mMoveCoord = coord;
        MR::startLevelSound(this, "SE_OJ_LV_METAL_LAND_MOVE");
    }

    void SpikeStick::exeWait() {
        if (mStepDelay > 0) {
            mStepDelay--;
            mSpine->mStep = 0;
            return;
        }

        if (MR::isStep(this, mStepWait)) {
            setNerve(GET_NERVE_ANON(NrvSpikeStickMoveStart));
        }
    }

    void SpikeStick::exeMoveStart() {
        if (MR::isFirstStep(this)) {
            MR::startSound(this, "SE_OJ_NUT_SHIP_BRIDGE_ST");
        }

        s32 stepMoveCharge = mStepMoveWait / 2;

        if (MR::isLessEqualStep(this, stepMoveCharge)) {
            updateMove(stepMoveCharge, 0.0f, ::sCoordMoveStart, MovementEasing_None);
        }

        if (MR::isStep(this, mStepMoveWait)) {
            setNerve(GET_NERVE_ANON(NrvSpikeStickMove));
        }
    }

    void SpikeStick::exeMove() {
        updateMove(mStepMove, ::sCoordMoveStart, 1.0f, MovementEasing_In);

        if (MR::isStep(this, mStepMove)) {
            setNerve(GET_NERVE_ANON(NrvSpikeStickMoveEnd));
        }
    }

    void SpikeStick::exeMoveEnd() {
        if (MR::isFirstStep(this)) {
            MR::startSound(this, "SE_OJ_METAL_LAND_STOP");
            MR::shakeCameraNormal();
        }

        if (MR::isStep(this, mStepMoveWait)) {
            setNerve(GET_NERVE_ANON(NrvSpikeStickMoveBack));
        }
    }

    void SpikeStick::exeMoveBack() {
        updateMove(mStepMoveBack, 1.0f, 0.0f, MovementEasing_InOut);

        if (MR::isStep(this, mStepMoveBack)) {
            // MR::shakeCameraVeryWeak();
            setNerve(GET_NERVE_ANON(NrvSpikeStickWait));
        }
    }
}; // namespace krp
