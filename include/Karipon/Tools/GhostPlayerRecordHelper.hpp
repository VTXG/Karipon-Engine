#pragma once

#include "Karipon/Tools/GstRecordHelper.hpp"

namespace gst {
    class GhostPlayerRecordHelper : public GstRecordHelper {
    public:
        GhostPlayerRecordHelper(const char* pName);

        virtual void makeActorAppeared();
        virtual GhostType getGhostType() const;
        virtual void initModelAndScene(const JMapInfoIter& rIter);
        virtual void handlePrepare();
        virtual void supplyGstFrame() const;

        const char* getModelName() const;
    };
}; // namespace gst
