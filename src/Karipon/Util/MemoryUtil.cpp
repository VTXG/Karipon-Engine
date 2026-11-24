#include "Karipon/Util/MemoryUtil.hpp"
#include <Game/System/HeapMemoryWatcher.hpp>
#include <Game/Util/SingletonHolder.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

namespace krp {
    bool isAppropriateHeapForArchive(const JKRHeap* pHeap) {
        HeapMemoryWatcher* heapMemoryWatcher = SingletonHolder< HeapMemoryWatcher >::get();
        return pHeap != static_cast< void* >(heapMemoryWatcher->mSceneHeapNapa);
    }

    void reportHeapForArchive(const char* pFilePath,const JKRHeap* pHeap) {
        if (pHeap == nullptr) {
            pHeap = JKRHeap::sCurrentHeap;
        }

        if (!isAppropriateHeapForArchive(pHeap)) {
            OSReport("File '%s' loaded into invalid heap '%s'\n", pFilePath, pHeap->mName);
        }
    }

    void memoryErrorCallback(const JKRHeap* pHeap, u32 size, int alignment) {
        const char* heapName;

        if (pHeap != nullptr && pHeap->mName != nullptr) {
            heapName = pHeap->mName;
        } else {
            heapName = "Unnamed";
        }

        OSPanic(__FILE__, __LINE__, "Failed to allocate memory in heap '%s' (size=%d, align=%d)", heapName, size, alignment);
    }
}; // namespace krp
