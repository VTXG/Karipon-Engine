#include "Karipon/AreaObj/PostFogArea.hpp"
#include <Game/Scene/SceneObjHolder.hpp>

namespace krp {
    PostFogArea::PostFogArea(int formType, const char* pName) : ImageEffectArea(ImageEffectArea::IMAGE_EFFECT_TYPE_POST_FOG, formType, pName) {}

    void PostFogArea::init(const JMapInfoIter& rIter) {
        ImageEffectArea::init(rIter);
        MR::createSceneObj(SceneObj_PostFog);
    }

    const char* PostFogArea::getManagerName() const {
        return "ImageEffectArea";
    }

    u16 PostFogArea::getMinRange() const {
        if (mObjArg0 < 0) {
            return 0;
        }

        if (mObjArg0 > 10000) {
            return 10000;
        }

        return mObjArg0;
    }

    u16 PostFogArea::getMaxRange() const {
        if (mObjArg1 < 0) {
            return 0;
        }

        if (mObjArg1 > 10000) {
            return 10000;
        }

        return mObjArg1;
    }

    u8 PostFogArea::getAlpha() const {
        if (mObjArg2 < 0) {
            return 0;
        }

        if (mObjArg2 > 0xFF) {
            return 0xFF;
        }

        return mObjArg2;
    }

    u8 PostFogArea::getRed() const {
        if (mObjArg3 < 0) {
            return 0;
        }

        if (mObjArg3 > 0xFF) {
            return 0xFF;
        }

        return mObjArg3;
    }

    u8 PostFogArea::getGreen() const {
        if (mObjArg4 < 0) {
            return 0;
        }

        if (mObjArg4 > 0xFF) {
            return 0xFF;
        }

        return mObjArg4;
    }

    u8 PostFogArea::getBlue() const {
        if (mObjArg5 < 0) {
            return 0;
        }

        if (mObjArg5 > 0xFF) {
            return 0xFF;
        }

        return mObjArg5;
    }

    s32 PostFogArea::getGradType() const {
        return mObjArg6;
    }
}; // namespace krp
