#include "Karipon/AudioLib/AudDataHolder.hpp"
#include "Karipon/Util/ByamlUtil.hpp"
#include <Game/Util/FileUtil.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>

namespace {
    static void attachResource(JKRMemArchive* pArchive, JMapInfo& rIter, const char* pResourcePath) {
        rIter.attach(pArchive->getResource(pResourcePath));
    }

    static void attachResource(JKRMemArchive* pArchive, krp::ByamlIter& rIter, const char* pResourcePath) {
        rIter = krp::createByamlRoot(pArchive->getResource(pResourcePath));
    }
}; // namespace

namespace krp {
    AudDataHolder::AudDataHolder() : mArchive(), mStageWaveTable() {}

    void AudDataHolder::init() {
        mArchive = MR::receiveArchive("/SystemData/AudDataTable.arc");

        ::attachResource(mArchive, mStageWaveTable, "StageWaveTable.byaml");
    }
}; // namespace krp
