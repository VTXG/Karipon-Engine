#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/os/OSTime.h>
#include <revolution/types.h>

namespace krp {
    enum OnlinePacketType {
        OnlinePacketType_Ping,
        OnlinePacketType_Pong,

        OnlinePacketType_RoomJoin,
        OnlinePacketType_RoomCreate,
        OnlinePacketType_RoomInfo,

        OnlinePacketType_PlayerJoin,
        OnlinePacketType_PlayerLeave,
        OnlinePacketType_PlayerData,
        OnlinePacketType_PlayerStage,
    };

    class OnlinePacketStream {
    public:
        OnlinePacketStream();

        void attach(u8* pData, u32 size, u32 capacity);
        void read(void* pBuffer, u32 size);
        void write(const void* pBuffer, u32 size);
        void seek(u32 position) {
            mPosition = position;
        }

        template < typename T >
        void read(T* pData) {
            read(static_cast< void* >(pData), sizeof(*pData));
        }

        template < typename T >
        void write(const T* pData) {
            write(static_cast< const void* >(pData), sizeof(*pData));
        }

        u32 getPosition() const {
            return mPosition;
        }

        u32 getSize() const {
            return mSize;
        }

        u32 getCapacity() const {
            return mCapacity;
        }

        bool isError() const {
            return mIsError;
        }

    private:
        u8* mData;
        u32 mPosition;
        u32 mSize;
        u32 mCapacity;
        bool mIsError;
    };

    struct OnlinePacketHeader {
        u32 mHash;
        u32 mType;
        OSTime mTimestamp;
        u8 mPlayerID;
    };
}; // namespace krp
