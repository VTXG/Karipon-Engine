#include "Game/NPC/CometEventKeeper.hpp"
#include "Game/NPC/CometEventExecutorTimeLimit.hpp"
#include "Game/Screen/GalaxyCometScreenFilter.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

#ifdef TARGET_KRP
#include <Karipon/System/SystemDataHolder.hpp>
#endif

#ifndef TARGET_KRP
struct GalaxyTimeLimitInfo {
    /* 0x0 */ const char* mGalaxyName;
    /* 0x4 */ s32 mScenarioNo;
    /* 0x8 */ u32 mTimeLimit;
};

namespace {
    static const GalaxyTimeLimitInfo sTimeLimitInfoTable[] = {
        {"EggStarGalaxy", 4, 240},      {"StarDustGalaxy", 4, 240}, {"SandClockGalaxy", 4, 270},
        {"CannonFleetGalaxy", 4, 360},  {"StarDustGalaxy", 5, 120}, {"PhantomGalaxy", 5, 60},
        {"CosmosGardenGalaxy", 5, 150}, {"FactoryGalaxy", 5, 180},  {"ReverseKingdomGalaxy", 5, 210},
    };
}; // namespace
#endif

CometEventKeeper::CometEventKeeper() : mExecutorTimeLimit(nullptr), mScreenFilter(nullptr), mCometName(nullptr), mCometStateIndex(0) {}

void CometEventKeeper::init() {
    initCometStatus();

#ifdef TARGET_KRP
    krp::ByamlIter params = krp::SystemDataHolder::getInstance()->getStageParams(MR::getCurrentStageName(), MR::getCurrentScenarioNo());
#endif

    if (isStartEvent("Red") || isStartEvent("Black")) {
        u32 timeLimit;

#ifdef TARGET_KRP
        timeLimit = 0;

        if (params.isValid()) {
            params.tryGetValueByKey("TimeLimit", &timeLimit);
        }
#else
        timeLimit = getTimeLimitFromTable(::sTimeLimitInfoTable, ARRAY_SIZE(::sTimeLimitInfoTable)) / 60;
#endif

        mExecutorTimeLimit = new CometEventExecutorTimeLimit(timeLimit);
        mExecutorTimeLimit->initWithoutIter();
        mExecutorTimeLimit->kill();
    }

    const char* cometFilterName = mCometName;

#ifdef TARGET_KRP
    if (params.isValid()) {
        params.tryGetValueByKey("CometFilter", &cometFilterName);
    }
#endif

    if (cometFilterName != nullptr) {
        mScreenFilter = new GalaxyCometScreenFilter();
        mScreenFilter->initWithoutIter();
        mScreenFilter->_20 = true;
        mScreenFilter->setCometType(cometFilterName);
    }
}

bool CometEventKeeper::isStartEvent(const char* pParam1) const {
    if (mCometName == nullptr) {
        return false;
    }

    return MR::isEqualString(mCometName, pParam1);
}

bool CometEventKeeper::isStartTimeLimitEvent() const {
    return mExecutorTimeLimit != nullptr;
}

void CometEventKeeper::startCometEventIfExist() {
    if (mExecutorTimeLimit == nullptr) {
        return;
    }

    mExecutorTimeLimit->appear();
}

void CometEventKeeper::endCometEvent() {
    if (mExecutorTimeLimit == nullptr) {
        return;
    }

    mExecutorTimeLimit->kill();
    mScreenFilter->_20 = false;
}

#ifndef TARGET_KRP
u32 CometEventKeeper::getTimeLimitFromTable(const GalaxyTimeLimitInfo* pTable, int size) {
    for (int i = 0; i < size; i++) {
        const GalaxyTimeLimitInfo* pInfo = &pTable[i];

        if (pInfo->mScenarioNo != MR::getCurrentScenarioNo()) {
            continue;
        }

        if (!MR::isEqualString(MR::getCurrentStageName(), pInfo->mGalaxyName)) {
            continue;
        }

        return pInfo->mTimeLimit * 60;
    }

    return 0;
}
#endif

void CometEventKeeper::initCometStatus() {
    mCometName = MR::makeCurrentGalaxyStatusAccessor().getCometName(MR::getCurrentScenarioNo());
    mCometStateIndex = MR::getGalaxyCometStateIndexInCurrentStage();
}
