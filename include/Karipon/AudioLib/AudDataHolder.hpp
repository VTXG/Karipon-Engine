#pragma once

#include "Karipon/Util/ByamlIter.hpp"
#include "Karipon/Util/GlobalInstance.hpp"
#include <Game/Util/JMapInfo.hpp>

class JKRMemArchive;

namespace krp {
    class AudDataHolder : public GlobalInstance< AudDataHolder > {
    public:
        AudDataHolder();

        void init();

        ByamlIter getBgmInfo(const char* pStageName) const;
        ByamlIter getBgmInfo(const char* pStageName, s32 scenarioNo) const;
        u32 getBgmId(const char* pStageName, s32 scenarioNo) const;
        s32 getBgmStartTime(const char* pStageName, s32 scenarioNo) const;
        bool isStartBgmOnWelcome(const char* pStageName, s32 scenarioNo) const;
        u32 getChangeBgmId(const char* pStageName, s32 index) const;
        u32 getChangeBgmState(const char* pStageName, s32 index) const;
        bool isPriorityBgm(const char* pStageName) const;

        JKRMemArchive* mArchive;
        krp::ByamlIter mStageBgmTable;
        krp::ByamlIter mStageWaveTable;
    };
}; // namespace krp
