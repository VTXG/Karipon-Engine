#pragma once

#include <Game/AreaObj/ImageEffectArea.hpp>

namespace krp {
    class PostFogArea : public ImageEffectArea {
    public:
        PostFogArea(int formType, const char* pName);

        virtual void init(const JMapInfoIter& rIter);
        virtual const char* getManagerName() const;

        u16 getMinRange() const;
        u16 getMaxRange() const;
        u8 getAlpha() const;
        u8 getRed() const;
        u8 getGreen() const;
        u8 getBlue() const;
        s32 getGradType() const;
    };
};
