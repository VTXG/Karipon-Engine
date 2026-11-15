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

        JKRMemArchive* mArchive;
        krp::ByamlIter mStageWaveTable;
    };
}; // namespace krp
