#include "Karipon/Tools/DebugPrint.hpp"
#include "Karipon/Tools/KeyboardFunction.hpp"
#include <Game/NameObj/NameObjHolder.hpp>
#include <Game/System/FileLoader.hpp>
#include <Game/System/GameSystem.hpp>
#include <Game/System/GameSystemObjHolder.hpp>
#include <Game/System/GameSystemSceneController.hpp>
#include <Game/System/WPad.hpp>
#include <Game/System/WPadButton.hpp>
#include <Game/System/WPadHolder.hpp>
#include <Game/Util/DirectDraw.hpp>
#include <Game/Util/GamePadUtil.hpp>
#include <Game/Util/ScreenUtil.hpp>
#include <Game/Util/SingletonHolder.hpp>
#include <Game/Util/SystemUtil.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <wstring.h>

extern "C" int vswprintf(wchar_t* s, size_t n, const wchar_t* format, va_list arg);

namespace {
    typedef void (*PageUpdateFunc)(krp::DebugPrint::Context& rContext);

    namespace DiagnosticsPage {
        static OSTick sLastTick;

        static void printHeapInfo(krp::DebugPrint::Context& rContext, JKRHeap* pHeap, s32 level) {
            if (pHeap == nullptr) {
                return;
            }

            u32 max = pHeap->mSize;
            u32 used = max - pHeap->getFreeSize();
            f32 usedPercent = static_cast< f32 >(used) / static_cast< f32 >(max) * 100.0f;

            char padding[32];
            if (level >= sizeof(padding)) {
                level = sizeof(padding) - 1;
            }

            for (s32 i = 0; i < level && i + 1 < sizeof(padding); i += 2) {
                padding[i] = ' ';
                padding[i + 1] = ' ';
            }

            padding[level] = '\0';

            const char* heapName = pHeap->mName;

            if (heapName == nullptr) {
                heapName = "Unnamed";
            }

            rContext.printTextF(false, L"%s%s : %.02f%% usage, 0x%08X/0x%08X bytes used\n", padding, heapName, usedPercent, used, max);

            const JSUTree< JKRHeap >& tree = pHeap->mChildTree;
            if (tree.getNumChildren() != 0) {
                for (JSUTreeIterator< JKRHeap > iterator(tree.getFirstChild()); iterator != tree.getEndChild(); ++iterator) {
                    printHeapInfo(rContext, iterator.getObject(), level + 1);
                }
            }
        }

        static void update(krp::DebugPrint::Context& rContext) {
            OSTick tick = OSGetTick();
            OSTick span = tick - sLastTick;
            sLastTick = tick;

            f32 fps = span == 0 ? 0.0f : static_cast< f32 >(OS_TIMER_CLOCK) / static_cast< f32 >(span);
            rContext.printTextF(false, L"FPS : %.0f\n", fps);

            FileLoader* pFileLoader = SingletonHolder< FileLoader >::get();
            ArchiveHolder* pArchiveHolder = pFileLoader->mArchiveHolder;
            FileHolder* pFileHolder = pFileLoader->mFileHolder;
            rContext.printTextF(false, L"File Info : %d requests, %d/%d archives, %d/%d files\n", pFileLoader->mRequestedFileCount,
                                pArchiveHolder->mEntries.mCount, pArchiveHolder->mEntries.capacity(), pFileHolder->mEntries.mCount,
                                pFileHolder->mEntries.capacity());

            NameObjHolder* pNameObjHolder = SingletonHolder< GameSystem >::get()->mSceneController->mObjHolder;
            rContext.printTextF(false, L"Object Info : %d/%d objects\n", pNameObjHolder->getObjArraySize(), pNameObjHolder->getObjArrayCapacity());
            printHeapInfo(rContext, JKRHeap::sRootHeap, 0);
        }
    } // namespace DiagnosticsPage

    namespace OnlinePage {
        static void update(krp::DebugPrint::Context& rContext) {}
    } // namespace OnlinePage

    static const f32 cFontSizeWidth = 10.0f;
    static const f32 cFontSizeHeight = 12.0f;
    static const f32 cFontLineSpace = 2.0f;
    static const f32 cFontCharSpace = 0.0f;

    struct PageUpdateInfo {
        const char* mName;
        const PageUpdateFunc mFunc;
    };

    static const PageUpdateInfo cPageUpdateFunc[] = {
        {"Diagnostics", DiagnosticsPage::update},
        {"Online", OnlinePage::update},
    };

    static s32 wrap(s32 num, s32 min, s32 max) {
        if (num < min) {
            num = max - 1;
        } else if (num >= max) {
            num = min;
        }

        return num;
    }
}; // namespace

namespace krp {
    DebugPrint::Context::Context(DebugPrint* pLayout) : mLayout(pLayout), mWriter(), mDrawRect() {
        nw4r::ut::Font* pFont = MR::getFontOnCurrentLanguage();

        mWriter.SetFont(*pFont);
        mWriter.SetFontSize(::cFontSizeWidth, ::cFontSizeHeight);
        mWriter.SetLineSpace(::cFontLineSpace);
        mWriter.SetCharSpace(::cFontCharSpace);

        mDrawRect.left = 0.0f;
        mDrawRect.top = 0.0f;
        mDrawRect.right = MR::getFrameBufferWidth();
        mDrawRect.bottom = MR::getFrameBufferHeight();

        mWriter.SetCursor(mDrawRect.left, mDrawRect.top);
    }

    void DebugPrint::Context::prepareDraw(u32 color) {
        Mtx mtx;
        PSMTXIdentity(mtx);

        mtx[0][3] = ((mDrawRect.left - mDrawRect.right) / 2.0f) + (mWriter.GetFontWidth() * 2.0f);
        mtx[1][1] = -1.0f;
        mtx[1][3] = ((mDrawRect.bottom - mDrawRect.top) / 2.0f) - mWriter.GetFontHeight();

        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);

        nw4r::ut::Color col(color);
        mWriter.SetGradationMode(nw4r::ut::CharWriter::GRADMODE_NONE);
        mWriter.SetTextColor(col);

        nw4r::ut::Color minCol(0x00000000);
        nw4r::ut::Color maxCol(color);
        mWriter.SetColorMapping(minCol, maxCol);
        mWriter.SetupGX();

        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    }

    bool DebugPrint::Context::printText(bool isSelectable, const wchar_t* pText) {
        if (pText == nullptr) {
            return false;
        }

        u32 len = wcslen(pText);
        if (len == 0) {
            return false;
        }

        nw4r::math::VEC3 cursorPos(mWriter.mCursorPos);

        mWriter.SetCursorX(cursorPos.x + 1.0f);
        mWriter.SetCursorY(cursorPos.y + 1.0f);
        prepareDraw(0x000000D0);
        mWriter.Print(pText, len);

        mWriter.mCursorPos = cursorPos;

        bool isSelected = false;

        if (isSelectable) {
            if (mLayout->mSelectIndex == mLayout->mSelectCurrentIndex) {
                prepareDraw(0x00FFDDFF);
                isSelected = krp::KeyboardFunction::isKeyPressed(krp::KBD_KEY_ENTER);
            } else {
                prepareDraw(0xFFFFFFFF);
            }

            mLayout->mSelectCurrentIndex++;
        } else {
            prepareDraw(0xFFFFFFFF);
        }

        mWriter.Print(pText, len);
        return isSelected;
    }

    bool DebugPrint::Context::printTextF(bool isSelectable, const wchar_t* pText, ...) {
        va_list list;
        wchar_t text[0x200];

        va_start(list, text);
        vswprintf(text, sizeof(text) / sizeof(*text), pText, list);
        va_end();

        return printText(isSelectable, text);
    }

    void DebugPrint::Context::printFillBox(const TVec2f& tl, const TVec2f& br, u32 color) {
        prepareDraw(0xFFFFFFFF);
        TDDraw::setup(0, 0, 2);
        TDDraw::drawFillBox(tl, br, color);
    }

    DebugPrint::DebugPrint() : mPageIndex(), mSelectIndex(), mSelectCurrentIndex(), mIsVisible() {
        krp::KeyboardFunction::init();
    }

    void DebugPrint::draw() {
        if (MR::testCorePadTrigger2(WPAD_CHAN0)) {
            krp::KeyboardFunction::update(); // flush existing inputs
            mIsVisible = !mIsVisible;
        }

        if (!mIsVisible) {
            return;
        }

        if (KeyboardFunction::getState() != krp::KeyboardState_Initialized) {
            Context ctx(this);
            ctx.printText(false, L"Keyboard not initialized");
            return;
        }

        krp::KeyboardFunction::update();
        mSelectCurrentIndex = 0;

        Context ctx(this);
        ctx.printTextF(false, L"----- %s [%d/%d] -----\n", ::cPageUpdateFunc[mPageIndex].mName, mPageIndex + 1, ARRAY_SIZE(::cPageUpdateFunc));
        ::cPageUpdateFunc[mPageIndex].mFunc(ctx);

        if (krp::KeyboardFunction::isKeyPressed(krp::KBD_KEY_W)) {
            mSelectIndex = wrap(mSelectIndex - 1, 0, mSelectCurrentIndex);
        }

        if (krp::KeyboardFunction::isKeyPressed(krp::KBD_KEY_S)) {
            mSelectIndex = wrap(mSelectIndex + 1, 0, mSelectCurrentIndex);
        }

        if (krp::KeyboardFunction::isKeyPressed(krp::KBD_KEY_A)) {
            mPageIndex = wrap(mPageIndex - 1, 0, ARRAY_SIZE(::cPageUpdateFunc));
            mSelectIndex = 0;
        }

        if (krp::KeyboardFunction::isKeyPressed(krp::KBD_KEY_D)) {
            mPageIndex = wrap(mPageIndex + 1, 0, ARRAY_SIZE(::cPageUpdateFunc));
            mSelectIndex = 0;
        }
    }
} // namespace krp
