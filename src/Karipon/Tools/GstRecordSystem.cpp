#include "Karipon/Tools/GstRecordSystem.hpp"

#define ADDR_GST_RECORDER_INFO_PTR (reinterpret_cast< GstRecorderInfo** >(0x80003FF8))

namespace gst {
    GstRecorderInfo::GstRecorderInfo()
        : mUpdateFrame(), mRecorderMode(), mCurrentStageName(), mCurrentStageScenario(), mIsSMG2(false), mGhostDataType(), mGhostDataStruct() {
        *ADDR_GST_RECORDER_INFO_PTR = this;
    }

    GstRecorderInfo(GstRecorderInfo::sInstance);

    // ----------------------------------------------------------------------------------------------------------------

    bool isEnableDebugGstRecorder() {
#ifdef DEBUG
        return true;
#else
        return false;
#endif
    }

    void waitGstRecorder() {
        GstRecorderInfo::sInstance.mRecorderMode = GstRecorderMode_Waiting;
    }

    void prepareGstRecorder(const char* stageName, s32 scenarioId, GhostType ghostType) {
        GstRecorderInfo::sInstance.mCurrentStageName = stageName;
        GstRecorderInfo::sInstance.mCurrentStageScenario = scenarioId;
        GstRecorderInfo::sInstance.mGhostDataType = ghostType;
        GstRecorderInfo::sInstance.mUpdateFrame = 0;
        GhostPacketSyncData(mGhostDataStruct);

        GstRecorderInfo::sInstance.mRecorderMode = GstRecorderMode_Preparing;
    }

    void startGstRecorder() {
        GstRecorderInfo::sInstance.mRecorderMode = GstRecorderMode_Recording;
    }

    void stopGstRecorder() {
        GstRecorderInfo::sInstance.mRecorderMode = GstRecorderMode_Stopped;
    }

    void supplyGstFrameSyncWithActor(const LiveActor* actor, bool syncAnims) {
        GstRecorderInfo::sInstance.mGhostDataStruct.syncWithActor(actor, syncAnims);
        GstRecorderInfo::sInstance.mUpdateFrame++;
    }

    void supplyGstFrameSyncWithPlayer() {
        GstRecorderInfo::sInstance.mGhostDataStruct.syncWithPlayer();
        GstRecorderInfo::sInstance.mUpdateFrame++;
    }

    void incGstFrame() {
        GstRecorderInfo::sInstance.mUpdateFrame++;
    }
}; // namespace gst
