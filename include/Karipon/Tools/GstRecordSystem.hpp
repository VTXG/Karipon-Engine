#pragma once

#include "Karipon/Tools/GhostPacketSyncData.hpp"
#include "Karipon/Tools/GhostType.hpp"

namespace gst {
    enum GstRecorderMode {
        GstRecorderMode_Waiting = 0,
        GstRecorderMode_Preparing = 1,
        GstRecorderMode_Recording = 2,
        GstRecorderMode_Stopped = 3
    };

    struct GstRecorderInfo {
        GstRecorderInfo();

        /* 0x00 */ u32 mUpdateFrame;
        /* 0x04 */ GstRecorderMode mRecorderMode;
        /* 0x08 */ const char* mCurrentStageName;
        /* 0x0C */ s32 mCurrentStageScenario;
        /* 0x10 */ bool mIsSMG2;
        /* 0x14 */ GhostType mGhostDataType;
        /* 0x18 */ GhostPacketSyncData mGhostDataStruct;

        static GstRecorderInfo sInstance;
    };

    bool isEnableDebugGstRecorder();

    void waitGstRecorder();
    void prepareGstRecorder(const char* stageName, s32 scenarioId, GhostType ghostType);
    void startGstRecorder();
    void stopGstRecorder();
    void supplyGstFrameSyncWithActor(const LiveActor* actor, bool syncAnims);
    void supplyGstFrameSyncWithPlayer();
    void incGstFrame();
}; // namespace gst
