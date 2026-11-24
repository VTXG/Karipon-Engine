#pragma once

#include <revolution/types.h>

class JKRHeap;

namespace krp {
    bool isAppropriateHeapForArchive(const JKRHeap* pHeap);
    void reportHeapForArchive(const char* pFilePath, const JKRHeap* pHeap);
    void memoryErrorCallback(const JKRHeap* pHeap, u32 size, int alignment);
}; // namespace krp
