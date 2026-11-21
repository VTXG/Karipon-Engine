#include "Karipon/Online/OnlinePlayer.hpp"
#include <Game/Animation/XanimeCore.hpp>
#include <Game/Animation/XanimePlayer.hpp>
#include <Game/LiveActor/ModelManager.hpp>
#include <Game/Util/ActorSensorUtil.hpp>
#include <Game/Util/ActorShadowUtil.hpp>
#include <Game/Util/DemoUtil.hpp>
#include <Game/Util/LiveActorUtil.hpp>
#include <Game/Util/ObjUtil.hpp>
#include <Game/Util/PlayerUtil.hpp>

namespace krp {
    OnlinePlayerState::OnlinePlayerState() {
        reset();
    }

    void OnlinePlayerState::createActor() {
        if (mActor == nullptr) {
            mActor = new OnlinePlayerActor(this);
            mActor->initWithoutIter();
        }
    }

    void OnlinePlayerState::destroyActor() {
        if (mActor != nullptr) {
            delete mActor;
            mActor = nullptr;
        }
    }

    void OnlinePlayerState::reset() {
        mStageName.reset();
        mScenarioNo = -1;
        mPosition.zero();
        mRotation.zero();
        mScale.zero();
        mAnimName.reset();
        mAnimNameHash = 0;
        mAnimFrame = 0.0f;
        mAnimWeights[0] = 0.0f;
        mAnimWeights[1] = 0.0f;
        mAnimWeights[2] = 0.0f;
        mAnimWeights[3] = 0.0f;
        mIsSimpleAnimation = false;
    }

    OnlinePlayerActor::OnlinePlayerActor(OnlinePlayerState* pState)
        : LiveActor("OnlinePlayerActor"), mState(pState), mXanimePlayer(), mXanimePlayerUpper() {}

    void OnlinePlayerActor::init(const JMapInfoIter& rIter) {
        mPosition.zero();
        mRotation.zero();
        mScale.set(1.0f);

        initModelManagerWithAnm("GhostMario", nullptr, true);

        XanimeResourceTable* pResourceTable = MR::getPlayerXanimeResource();
        mXanimePlayer = new XanimePlayer(mModelManager->getJ3DModel(), pResourceTable);
        mXanimePlayer->duplicateSimpleGroup();
        mModelManager->mXanimePlayer = mXanimePlayer;
        mXanimePlayer->setDefaultAnimation("基本");
        mXanimePlayer->getCore()->enableJointTransform(mModelManager->getJ3DModelData());
        mXanimePlayerUpper = new XanimePlayer(mModelManager->getJ3DModel(), pResourceTable, mXanimePlayer);
        mXanimePlayerUpper->changeAnimation("基本");

        initHitSensor(1);
        MR::addHitSensorMapObj(this, "body", 32, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));

        MR::initShadowVolumeSphere(this, 50.0f);
        MR::validateShadow(this, nullptr);
        MR::calcGravity(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropGravity(this, nullptr);

        MR::invalidateClipping(this);
        MR::connectToSceneMapObjStrongLight(this);
        MR::registerDemoSimpleCastAll(this);

        MR::connectToSceneMapObj(this);
        makeActorAppeared();

        MR::startBtk(this, "GhostMario");
        MR::startBrk(this, "GhostMario");
        // updateVisibility();
    }
}; // namespace krp
