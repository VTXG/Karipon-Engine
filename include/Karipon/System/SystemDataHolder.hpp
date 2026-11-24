#pragma once

#include "Karipon/Util/ByamlIter.hpp"
#include "Karipon/Util/GlobalInstance.hpp"
#include <Game/Util/Array.hpp>
#include <Game/Util/JMapInfo.hpp>

class GalaxyCometTimePaper;
class GameEventFlag;
class GameEventValue;
class JKRMemArchive;

namespace krp {
    struct RaceData {
        s32 getMaxTime() const {
            return mMaxTime;
        }

        /* 0x00 */ const char* mName;
        /* 0x04 */ const char* mBgmIdName;
        /* 0x08 */ s32 mMaxTime;
        /* 0x0C */ const char* mMessageId;
        /* 0x10 */ const char* mGalaxyName;
        /* 0x14 */ s32 mScenarioNo;
        /* 0x18 */ bool mIsDemoWithStarPointer;
    };

    class SystemDataHolder : public GlobalInstance< SystemDataHolder > {
    public:
        SystemDataHolder();

        void init();
        void initCometTable();
        void initEventFlagTable(const JMapInfo* pInfo);
        void initEventValueTable(const JMapInfo* pInfo);
        void initRaceDataTable(const JMapInfo* pInfo);

        JMapInfoIter getGalaxyInfo(const char* pName) const;
        u8 getStoryEventProgress(const char* pName) const;
        krp::ByamlIter getStageParams(const char* pName, s32 scenarioNo = 0) const;
        krp::ByamlIter getDomeParams(s32 scenarioNo) const;
        const RaceData& getRaceData(s32 id) const;
        s32 getRaceId(const char* pGalaxyName, s32 scenarioNo) const;

        JKRMemArchive* mArchive;
        JMapInfo mGalaxyInfoTable;
        MR::AssignableArray< GalaxyCometTimePaper* > mCometTable;
        MR::AssignableArray< GameEventFlag > mEventFlagTable;
        MR::AssignableArray< GameEventValue > mEventValueTable;
        JMapInfo mStoryEventTable;
        krp::ByamlIter mStageParamTable;
        krp::ByamlIter mDomeParamTable;
        MR::AssignableArray< RaceData > mRaceDataTable;
    };
}; // namespace krp
