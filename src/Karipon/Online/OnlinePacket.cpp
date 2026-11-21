#include "Karipon/Online/OnlinePacket.hpp"
#include <cstdio>

namespace krp {
    void copyNetworkString(char* pBuff, const char* pStr, u32 size) {
        int len = snprintf(pBuff, size, "%s", pStr) + 1;
        pBuff[len >= size ? size - 1 : len] = '\0';
    }

    OnlinePacket::OnlinePacket() {
        reset();
    }

    void OnlinePacket::reset() {
        mSize = 0;
        mIsError = false;
        seek(0);
    }

    void OnlinePacket::read(void* pBuffer, u32 size) {
        if (size == 0) {
            return;
        }

        u32 newPosition = mPosition + size;
        if (newPosition > mSize) {
            mPosition = mSize;
            mIsError = true;
            return;
        }

        memcpy(pBuffer, mData + mPosition, size);
    }

    void OnlinePacket::write(const void* pBuffer, u32 size) {
        if (size == 0) {
            return;
        }

        u32 newPosition = mPosition + size;
        if (newPosition > sizeof(mData)) {
            mPosition = mSize;
            mIsError = true;
            return;
        }

        memcpy(mData + mPosition, pBuffer, size);

        if (mSize < newPosition) {
            mSize = newPosition;
        }
    }
}; // namespace krp
