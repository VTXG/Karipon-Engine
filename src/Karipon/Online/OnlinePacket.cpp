#include "Karipon/Online/OnlinePacket.hpp"
#include <cstring>

namespace krp {
    OnlinePacketStream::OnlinePacketStream() : mData(), mPosition(), mSize(), mCapacity(), mIsError() {}

    void OnlinePacketStream::attach(u8* pData, u32 size, u32 capacity) {
        mData = pData;
        mPosition = 0;
        mSize = size;
        mCapacity = capacity;
        mIsError = false;
    }

    void OnlinePacketStream::read(void* pBuffer, u32 size) {
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

    void OnlinePacketStream::write(const void* pBuffer, u32 size) {
        if (size == 0) {
            return;
        }

        u32 newPosition = mPosition + size;
        if (newPosition > mCapacity) {
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
