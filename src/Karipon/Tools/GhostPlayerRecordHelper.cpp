#include "Karipon/Tools/GhostPlayerRecordHelper.hpp"
#include "Karipon/Tools/GstRecordSystem.hpp"
#include <Game/Player/MarioAccess.hpp>
#include <Game/Scene/SceneFunction.hpp>
#include <Game/Util/EventUtil.hpp>
#include <Game/Util/LiveActorUtil.hpp>
#include <Game/Util/ObjUtil.hpp>
#include <Game/Util/PlayerUtil.hpp>

namespace gst {
    GhostPlayerRecordHelper::GhostPlayerRecordHelper(const char* pName) : GstRecordHelper(pName) {}

    void GhostPlayerRecordHelper::makeActorAppeared() {
        GstRecordHelper::makeActorAppeared();

        const char* modelName = getModelName();
        MR::startBtk(this, modelName);
        MR::startBrk(this, modelName);
    }

    GhostType GhostPlayerRecordHelper::getGhostType() const {
        return MR::isPlayerLuigi() ? GhostType_GhostPlayerMario : GhostType_GhostPlayerLuigi;
    }

    void GhostPlayerRecordHelper::supplyGstFrame() const {
        supplyGstFrameSyncWithPlayer();
    }

    void GhostPlayerRecordHelper::initModelAndScene(const JMapInfoIter& rIter) {
        initModelManagerWithAnm(getModelName(), nullptr, false);
        MR::connectToScene(this, MR::MovementType_NPC, MR::CalcAnimType_None, MR::DrawBufferType_IndirectNpc, MR::DrawType_None);
    }

    void GhostPlayerRecordHelper::handlePrepare() {
        MarioAccess::setBaseMtx(getBaseMtx());
        MarioAccess::setStateWait();
        MR::startBckPlayer("Wait");
        MR::hideModel(this);
    }

    const char* GhostPlayerRecordHelper::getModelName() const {
        return MR::isPlayerLuigi() ? "GhostLuigi" : "GhostMario";
    }
}; // namespace gst
