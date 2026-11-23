#include "Karipon/AudioLib/AudDataHolder.hpp"
#include "Karipon/Util/ByamlIter.hpp"
#include "Karipon/Util/ByamlUtil.hpp"
#include <Game/AudioLib/AudSoundNameConverter.hpp>
#include <Game/Util/FileUtil.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <cstdio>

namespace {
    static void attachResource(JKRMemArchive* pArchive, JMapInfo& rIter, const char* pResourcePath) {
        rIter.attach(pArchive->getResource(pResourcePath));
    }

    static void attachResource(JKRMemArchive* pArchive, krp::ByamlIter& rIter, const char* pResourcePath) {
        rIter = krp::createByamlRoot(pArchive->getResource(pResourcePath));
    }

    static u32 getSoundID(const char* pName) {
        if (pName == nullptr) {
            return -1;
        }

        return AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName).mID.mComposite;
    }
}; // namespace

namespace krp {
    AudDataHolder::AudDataHolder() : mArchive(), mStageBgmTable(), mStageWaveTable() {}

    void AudDataHolder::init() {
        mArchive = MR::receiveArchive("/SystemData/AudDataTable.arc");

        ::attachResource(mArchive, mStageBgmTable, "StageBgmTable.byaml");
        ::attachResource(mArchive, mStageWaveTable, "StageWaveTable.byaml");
    }

    ByamlIter AudDataHolder::getBgmInfo(const char* pStageName) const {
        return mStageBgmTable[pStageName];
    }

    ByamlIter AudDataHolder::getBgmInfo(const char* pStageName, s32 scenarioNo) const {
        char scenarioKey[32];
        snprintf(scenarioKey, sizeof(scenarioKey), "Scenario%d", scenarioNo);
        return mStageBgmTable[pStageName][scenarioKey];
    }

    u32 AudDataHolder::getBgmId(const char* pStageName, s32 scenarioNo) const {
        const char* name;
        ByamlIter iter = getBgmInfo(pStageName, scenarioNo);

        if (iter.isValid() && iter.tryGetValueByKey("BgmIdName", &name)) {
            return name != nullptr ? getSoundID(name) : -1;
        }

        if (scenarioNo != 0) {
            return getBgmId(pStageName, 0);
        }

        return nullptr;
    }

    s32 AudDataHolder::getBgmStartTime(const char* pStageName, s32 scenarioNo) const {
        u32 bgmStartTime;
        ByamlIter iter = getBgmInfo(pStageName, scenarioNo);

        if (iter.isValid() && iter.tryGetValueByKey("BgmStartTime", &bgmStartTime)) {
            return bgmStartTime;
        }

        if (scenarioNo != 0) {
            return getBgmStartTime(pStageName, 0);
        }

        return 0;
    }

    bool AudDataHolder::isStartBgmOnWelcome(const char* pStageName, s32 scenarioNo) const {
        bool startBgmOnWelcome;
        ByamlIter iter = getBgmInfo(pStageName, scenarioNo);

        if (iter.isValid() && iter.tryGetValueByKey("IsStartBgmOnWelcome", &startBgmOnWelcome)) {
            return startBgmOnWelcome;
        }

        if (scenarioNo != 0) {
            return isStartBgmOnWelcome(pStageName, 0);
        }

        return false;
    }

    u32 AudDataHolder::getChangeBgmId(const char* pStageName, s32 index) const {
        ByamlIter iter = getBgmInfo(pStageName)["ChangeBgmIdName"];

        const char* name = nullptr;
        iter.tryGetValueByIndex(index, &name);
        return getSoundID(name);
    }

    u32 AudDataHolder::getChangeBgmState(const char* pStageName, s32 index) const {
        ByamlIter iter = getBgmInfo(pStageName)["ChangeBgmState"];

        u32 state = -1;
        iter.tryGetValueByIndex(index, &state);
        return state;
    }

    bool AudDataHolder::isPriorityBgm(const char* pStageName) const {
        ByamlIter iter = getBgmInfo(pStageName);

        bool isPriority = false;
        iter.tryGetValueByKey("IsPriority", &isPriority);
        return isPriority;
    }
}; // namespace krp
