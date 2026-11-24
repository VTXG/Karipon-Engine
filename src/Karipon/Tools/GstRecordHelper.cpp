#include "Karipon/Tools/GstRecordHelper.hpp"
#include "Karipon/Tools/GstRecordSystem.hpp"
#include <Game/LiveActor/Nerve.hpp>
#include <Game/Screen/IconAButton.hpp>
#include <Game/Util/ActorSensorUtil.hpp>
#include <Game/Util/GamePadUtil.hpp>
#include <Game/Util/LiveActorUtil.hpp>
#include <Game/Util/ObjUtil.hpp>
#include <Game/Util/PlayerUtil.hpp>
#include <Game/Util/SceneUtil.hpp>

namespace {
    NEW_NERVE(GstRecordHelperNrvWait, gst::GstRecordHelper, Wait);
    NEW_NERVE(GstRecordHelperNrvPreparing, gst::GstRecordHelper, Preparing);
    NEW_NERVE(GstRecordHelperNrvRecording, gst::GstRecordHelper, Recording);
}; // namespace

namespace gst {
    GstRecordHelper::GstRecordHelper(const char* pName) : LiveActor(pName) {
        mIconAButton = NULL;
        mShowIconAButton = false;
    }

    void GstRecordHelper::init(const JMapInfoIter& rIter) {
        MR::initDefaultPos(this, rIter);
        initModelAndScene(rIter);

        initHitSensor(1);
        MR::addHitSensorMapObjSimple(this, "Body", 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));

        mIconAButton = new IconAButton(true, false);
        mIconAButton->initWithoutIter();

        initNerve(GET_NERVE_ANON(GstRecordHelperNrvWait));

        s32 stateNum = getStateCount();
        if (stateNum > 0) {
            mSpine->initStateKeeper(stateNum);
        }

        makeActorAppeared();
    }

    void GstRecordHelper::initModelAndScene(const JMapInfoIter& rIter) {
        MR::connectToSceneNpcMovement(this);
    }

    s32 GstRecordHelper::getStateCount() const {
        return 0;
    }

    void GstRecordHelper::makeActorAppeared() {
        if (isEnableDebugGstRecorder()) {
            LiveActor::makeActorAppeared();
        } else {
            LiveActor::makeActorDead();
        }
    }

    void GstRecordHelper::control() {
        mShowIconAButton = false;
    }

    void GstRecordHelper::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
        if (isNerve(GET_NERVE_ANON(GstRecordHelperNrvWait))) {
            if (MR::isSensorPlayer(pReceiver) && MR::isOnGroundPlayer()) {
                mShowIconAButton = true;
            }
        }
    }

    bool GstRecordHelper::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        if (MR::isMsgStartPowerStarGet(msg)) {
            forceStop();
            incGstFrame();
            return true;
        }

        return false;
    }

    void GstRecordHelper::exeWait() {
        if (mShowIconAButton) {
            if (!mIconAButton->isOpen()) {
                mIconAButton->openWithRead();
            }
        } else if (mIconAButton->isOpen()) {
            mIconAButton->term();
        }

        if (mShowIconAButton && mIconAButton->isOpen() && MR::testCorePadTriggerA(0)) {
            setNerve(GET_NERVE_ANON(GstRecordHelperNrvPreparing));
        }
    }

    void GstRecordHelper::exePreparing() {
        if (MR::isFirstStep(this)) {
            mIconAButton->term();

            MR::invalidateClipping(this);
            MR::offPlayerControl();
            handlePrepare();

            prepareGstRecorder(MR::getCurrentStageName(), MR::getCurrentSelectedScenarioNo(), getGhostType());
        }

        if (MR::isStep(this, 60)) {
            setNerve(GET_NERVE_ANON(GstRecordHelperNrvRecording));
        }
    }

    void GstRecordHelper::exeRecording() {
        if (MR::isFirstStep(this)) {
            MR::onPlayerControl(true);
            startGstRecorder();
        }

        supplyGstFrame();

        if (MR::testCorePadTrigger2(WPAD_CHAN0)) {
            forceStop();
        }
    }

    void GstRecordHelper::forceStop() {
        stopGstRecorder();
        handleStopped();
        makeActorDead();
    }

    void GstRecordHelper::handlePrepare() {}

    void GstRecordHelper::handleStopped() {}

    void GstRecordHelper::supplyGstFrame() const {
        supplyGstFrameSyncWithActor(this, true);
    }
}; // namespace gst
