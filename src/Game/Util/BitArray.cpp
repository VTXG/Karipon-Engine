#include "Game/Util/BitArray.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <Karipon/Globals.hpp>

namespace MR {
    BitArray::BitArray(int bitNum) : mArray(nullptr), mArraySize(bitNum) {
        int byteNum = TARGET_KRP_IF_ELSE(sizeU8(), (bitNum + 7 & ~7) / 8);

        mArray = new u8[byteNum];

        MR::zeroMemory(mArray, byteNum);
    }

    bool BitArray::isOn(int bitIdx) const {
        u8 byte = mArray[bitIdx / 8];

        return (byte & (1 << (bitIdx & 0x7))) != 0;
    }

    void BitArray::set(int bitIdx, bool isOn) {
        int byteIdx = bitIdx / 8;
        u8* pArray = mArray;

        if (isOn) {
            pArray[byteIdx] |= (1 << (bitIdx & 0x7));
        } else {
            pArray[byteIdx] &= ~(1 << (bitIdx & 0x7));
        }
    }
};  // namespace MR
