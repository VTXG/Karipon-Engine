#include "Game/System/GalaxyNameSortTable.hpp"
#include "Game/System/GalaxyInfoTable.hpp"
#include "Game/Util/JMapInfo.hpp"

#ifdef TARGET_KRP
#include <Karipon/System/SystemDataHolder.hpp>
#endif

s32 GalaxyNameSortTable::getGalaxySortIndex(const char* pName) {
#ifdef TARGET_KRP
    JMapInfoIter iter = krp::SystemDataHolder::getInstance()->getGalaxyInfo(pName);

    if (!iter.isValid()) {
        return -1;
    }
#else
    JMapInfo info;
    info.attach(&GalaxyInfoTable_bcsv_start);
    JMapInfoIter iter = info.findElement("name", pName, 0);

    if (iter == info.end()) {
        return -1;
    }
#endif

    return iter.mIndex;
}
