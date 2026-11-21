#pragma once

#include <private/iostypes.h>
#include <revolution/types.h>

namespace krp {
    enum KBDReturnCode {
        KBD_ERR_OK = 0,
        KBD_ERR_FATAL = -1,
        KBD_ERR_ALREADY_INIT = -2,
        KBD_ERR_NOT_INIT = -3,
        KBD_ERR_FAIL_INIT = -4,
        KBD_ERR_FAIL_ASYNC_INIT = -6,
    };

    enum KBDModifier {
        KBD_MOD_L_CTRL = 0x1,
        KBD_MOD_L_SHIFT = 0x2,
        KBD_MOD_L_ALT = 0x4,
        KBD_MOD_L_SUPER = 0x8,
        KBD_MOD_R_CTRL = 0x10,
        KBD_MOD_R_SHIFT = 0x20,
        KBD_MOD_R_ALT = 0x40,
        KBD_MOD_R_SUPER = 0x80,
        KBD_MOD_CTRL = KBD_MOD_L_CTRL | KBD_MOD_R_CTRL,
        KBD_MOD_SHIFT = KBD_MOD_L_SHIFT | KBD_MOD_R_SHIFT,
        KBD_MOD_ALT = KBD_MOD_L_ALT | KBD_MOD_R_ALT,
        KBD_MOD_SUPER = KBD_MOD_L_SUPER | KBD_MOD_R_SUPER,
    };

    enum KBDKey {
        KBD_KEY_A = 0x04,
        KBD_KEY_B = 0x05,
        KBD_KEY_C = 0x06,
        KBD_KEY_D = 0x07,
        KBD_KEY_E = 0x08,
        KBD_KEY_F = 0x09,
        KBD_KEY_G = 0x0A,
        KBD_KEY_H = 0x0B,
        KBD_KEY_I = 0x0C,
        KBD_KEY_J = 0x0D,
        KBD_KEY_K = 0x0E,
        KBD_KEY_L = 0x0F,
        KBD_KEY_M = 0x10,
        KBD_KEY_N = 0x11,
        KBD_KEY_O = 0x12,
        KBD_KEY_P = 0x13,
        KBD_KEY_Q = 0x14,
        KBD_KEY_R = 0x15,
        KBD_KEY_S = 0x16,
        KBD_KEY_T = 0x17,
        KBD_KEY_U = 0x18,
        KBD_KEY_V = 0x19,
        KBD_KEY_W = 0x1A,
        KBD_KEY_X = 0x1B,
        KBD_KEY_Y = 0x1C,
        KBD_KEY_Z = 0x1D,
        KBD_KEY_1 = 0x1F,
        KBD_KEY_2 = 0x20,
        KBD_KEY_3 = 0x21,
        KBD_KEY_4 = 0x22,
        KBD_KEY_5 = 0x23,
        KBD_KEY_6 = 0x24,
        KBD_KEY_7 = 0x25,
        KBD_KEY_8 = 0x26,
        KBD_KEY_9 = 0x27,
        KBD_KEY_ENTER = 0x28,
        KBD_KEY_ESCAPE = 0x29,
        KBD_KEY_BACKSPACE = 0x2A,
        KBD_KEY_TAB = 0x2B,
        KBD_KEY_SPACE = 0x2C,
        KBD_KEY_MINUS = 0x2D,
        KBD_KEY_EQUAL = 0x2E,
        KBD_KEY_LEFT_BRACKET = 0x2F,
        KBD_KEY_RIGHT_BRACKET = 0x30,
        KBD_KEY_NON_US_HASH = 0x31,
        KBD_KEY_NON_US_BACKSLASH = 0x32,
        KBD_KEY_SEMICOLON = 0x33,
        KBD_KEY_APOSTROPHE = 0x34,
        KBD_KEY_GRAVE = 0x35,
        KBD_KEY_COMMA = 0x36,
        KBD_KEY_PERIOD = 0x37,
        KBD_KEY_SLASH = 0x38,
        KBD_KEY_CAPS_LOCK = 0x39,
        KBD_KEY_F1 = 0x3A,
        KBD_KEY_F2 = 0x3B,
        KBD_KEY_F3 = 0x3C,
        KBD_KEY_F4 = 0x3D,
        KBD_KEY_F5 = 0x3E,
        KBD_KEY_F6 = 0x3F,
        KBD_KEY_F7 = 0x40,
        KBD_KEY_F8 = 0x41,
        KBD_KEY_F9 = 0x42,
        KBD_KEY_F10 = 0x43,
        KBD_KEY_F11 = 0x44,
        KBD_KEY_F12 = 0x45,
        KBD_KEY_PRINT_SCREEN = 0x46,
        KBD_KEY_SCROLL_LOCK = 0x47,
        KBD_KEY_PAUSE = 0x48,
        KBD_KEY_INSERT = 0x49,
        KBD_KEY_HOME = 0x4A,
        KBD_KEY_PAGE_UP = 0x4B,
        KBD_KEY_DELETE = 0x4C,
        KBD_KEY_END = 0x4D,
        KBD_KEY_PAGE_DOWN = 0x4E,
        KBD_KEY_ARROW_RIGHT = 0x4F,
        KBD_KEY_ARROW_LEFT = 0x50,
        KBD_KEY_ARROW_DOWN = 0x51,
        KBD_KEY_ARROW_UP = 0x52,
        KBD_KEY_NUM_LOCK = 0x53,
        KBD_KEY_NUMPAD_DIVIDE = 0x54,
        KBD_KEY_NUMPAD_MULTIPLY = 0x55,
        KBD_KEY_NUMPAD_SUBTRACT = 0x56,
        KBD_KEY_NUMPAD_ADD = 0x57,
        KBD_KEY_NUMPAD_ENTER = 0x58,
        KBD_KEY_NUMPAD_1 = 0x59,
        KBD_KEY_NUMPAD_2 = 0x5A,
        KBD_KEY_NUMPAD_3 = 0x5B,
        KBD_KEY_NUMPAD_4 = 0x5C,
        KBD_KEY_NUMPAD_5 = 0x5D,
        KBD_KEY_NUMPAD_6 = 0x5E,
        KBD_KEY_NUMPAD_7 = 0x5F,
        KBD_KEY_NUMPAD_8 = 0x60,
        KBD_KEY_NUMPAD_9 = 0x61,
        KBD_KEY_NUMPAD_0 = 0x62,
        KBD_KEY_NUMPAD_DECIMAL = 0x63,
        KBD_KEY_NON_US_BACKSLASH_2 = 0x64,
        KBD_KEY_APPLICATION = 0x65,
    };

    struct KBDData {
        /* 0x0 */ u32 mMessageType;
        /* 0x4 */ u32 _4;
        /* 0x8 */ u8 mModifiers;
        /* 0x9 */ u8 _9;
        /* 0xA */ u8 mKeys[6];

        void reset();
        void report() const;
        bool isOnModifier(KBDModifier modifier) const;
        bool isOnKey(KBDKey key) const;
    };

    enum KeyboardState {
        KeyboardState_Uninitialized,
        KeyboardState_Initializing,
        KeyboardState_Initialized,
    };

    class KeyboardFunction {
    public:
        static void init(bool wait = false);
        static void close();
        static IOSError getError();
        static KeyboardState getState();

        static void enable();
        static void disable();
        static void update();
        static bool isModifierPressed(KBDModifier modifier);
        static bool isModifierHold(KBDModifier modifier);
        static bool isKeyPressed(KBDKey key);
        static bool isKeyHold(KBDKey key);
    };
}; // namespace krp
