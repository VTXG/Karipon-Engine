#pragma once

#include "Karipon/Tools/GhostType.hpp"
#include <Game/LiveActor/LiveActor.hpp>

class IconAButton;

namespace gst {
    class GstRecordHelper : public LiveActor {
    public:
        GstRecordHelper(const char* pName);

        virtual void init(const JMapInfoIter& rIter);
        virtual void makeActorAppeared();
        virtual void control();
        virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
        virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

        virtual GhostType getGhostType() const = 0;
        virtual s32 getStateCount() const;
        virtual void initModelAndScene(const JMapInfoIter& rIter);
        virtual void handlePrepare();
        virtual void handleStopped();
        virtual void supplyGstFrame() const;

        void forceStop();

        void exeWait();
        void exePreparing();
        void exeRecording();

        IconAButton* mIconAButton;
        bool mShowIconAButton;
    };
}; // namespace gst
