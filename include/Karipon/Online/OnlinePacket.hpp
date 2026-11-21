#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/os/OSTime.h>
#include <revolution/types.h>

namespace krp {
    void copyNetworkString(char* pBuff, const char* pStr, u32 size);

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

    template < u32 Size >
    struct OnlineString {
        void set(const char* pStr) {
            copyNetworkString(mData, pStr, sizeof(mData));
        }

        void reset() {
            mData[0] = '\0';
        }

        /* 0x0 */ char mData[Size];
    };

    struct OnlinePacketHeader {
        /* 0x00 */ u32 mHash;
        /* 0x04 */ u32 mType;
        /* 0x08 */ OSTime mTimestamp;
        /* 0x10 */ u8 mPlayerID;
    };

    class OnlinePacket {
    public:
        static const int PACKET_HEADER_SIZE = sizeof(OnlinePacketHeader);
        static const int PACKET_BUFFER_SIZE = 0x100;

        OnlinePacket();

        void reset();
        void read(void* pBuffer, u32 size);
        void write(const void* pBuffer, u32 size);
        void seek(u32 position) {
            mPosition = position;
        }

    private:
        u8 mData[PACKET_BUFFER_SIZE] ATTRIBUTE_ALIGN(32);

        u32 mPosition;
        u32 mSize;
        bool mIsError;
    };
}; // namespace krp
