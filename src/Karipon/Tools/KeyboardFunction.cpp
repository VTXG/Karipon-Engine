#include "Karipon/Tools/KeyboardFunction.hpp"
#include <Game/Util/Functor.hpp>
#include <Game/Util/SystemUtil.hpp>
#include <revolution/ipc.h>

// https://github.com/koopthekoopa/wii-ipl/blob/main/libs/RVL_SDK/src/usbkbd/usb_kbd.c

enum {
    IOCTL_KBD_INIT_CALLBACK = 0x0,
};

enum {
    KBD_STATE_ATTACH = 0x0,
    KBD_STATE_DETACH = 0x1,
    KBD_STATE_EVENT = 0x2,
};

namespace {
    static IOSFd sFd = -1;
    static IOSError sError = krp::KBD_ERR_OK;
    static krp::KeyboardState sState = krp::KeyboardState_Uninitialized;
    static krp::KBDData sData ATTRIBUTE_ALIGN(32);
    static krp::KBDData sLastData ATTRIBUTE_ALIGN(32);

    void resetInternalState(krp::KeyboardState state) {
        sFd = -1;
        sError = krp::KBD_ERR_OK;
        sState = state;
        sData.reset();
        sLastData.reset();
    }

    void initProc() {
        BOOL interrupts = OSDisableInterrupts();
        IOSError err;

        // KBD initialization
        {
            IOSFd kbdFd = IOS_Open("/dev/usb/kbd", 0);
            if (kbdFd < 0) {
                err = kbdFd;
                goto Error;
            }

            sFd = kbdFd;
        }

        OSRestoreInterrupts(interrupts);
        goto Success;

    Error:
        sFd = -1;
        sError = err;
        sState = krp::KeyboardState_Uninitialized;
        OSRestoreInterrupts(interrupts);
        return;

    Success:
        sError = krp::KBD_ERR_OK;
        sState = krp::KeyboardState_Initialized;
        return;
    }
}; // namespace

namespace krp {
    void KBDData::reset() {
        mMessageType = 0;
        _4 = 0;
        mModifiers = 0;
        _9 = 0;

        for (s32 i = 0; i < sizeof(mKeys); i++) {
            mKeys[i] = 0;
        }
    }

    void KBDData::report() const {
        OSReport("mMessageType : %04X\n", sData.mMessageType);
        OSReport("mModifiers   : %04X\n", sData.mModifiers);
        OSReport("mKeys        : %02X %02X %02X %02X %02X %02X\n", sData.mKeys[0], sData.mKeys[1], sData.mKeys[2], sData.mKeys[3], sData.mKeys[4],
                 sData.mKeys[5]);
    }

    bool KBDData::isOnModifier(KBDModifier modifier) const {
        return (mModifiers & modifier) == modifier;
    }

    bool KBDData::isOnKey(KBDKey key) const {
        for (s32 i = 0; i < sizeof(mKeys); i++) {
            if (mKeys[i] == 0) {
                break;
            }

            if (mKeys[i] == key) {
                return true;
            }
        }

        return false;
    }

    void KeyboardFunction::init(bool wait) {
        if (sState != krp::KeyboardState_Uninitialized) {
            return;
        }

        resetInternalState(krp::KeyboardState_Initializing);
        MR::startFunctionAsyncExecute(MR::Functor(&initProc), 10, "KeyboardFunction::init");

        if (wait) {
            MR::waitForEndFunctionAsyncExecute("KeyboardFunction::init");
        }
    }

    void KeyboardFunction::close() {
        if (sFd >= 0) {
            IOS_Close(sFd);
        }

        resetInternalState(krp::KeyboardState_Uninitialized);
    }

    IOSError KeyboardFunction::getError() {
        return sError;
    }

    KeyboardState KeyboardFunction::getState() {
        return sState;
    }

    void KeyboardFunction::update() {
        if (sState != krp::KeyboardState_Initialized) {
            return;
        }

        sLastData = sData;
        IOS_Ioctl(sFd, IOCTL_KBD_INIT_CALLBACK, nullptr, 0, &sData, sizeof(sData));
    }

    bool KeyboardFunction::isModifierPressed(KBDModifier modifier) {
        return sData.isOnModifier(modifier) && !sLastData.isOnModifier(modifier);
    }

    bool KeyboardFunction::isModifierHold(KBDModifier modifier) {
        return sData.isOnModifier(modifier);
    }

    bool KeyboardFunction::isKeyPressed(KBDKey key) {
        return sData.isOnKey(key) && !sLastData.isOnKey(key);
    }

    bool KeyboardFunction::isKeyHold(KBDKey key) {
        return sData.isOnKey(key);
    }
}; // namespace krp
