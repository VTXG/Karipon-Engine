#pragma once

#include "Game/LiveActor/LiveActor.hpp"

#ifdef TARGET_KRP
#include <Karipon/Util/ByamlIter.hpp>
#endif

class LiveActorGroup;
class MiniatureGalaxy;

class MiniatureGalaxyHolder : public LiveActor {
public:
    MiniatureGalaxyHolder();

    virtual void init(const JMapInfoIter&);

    void registerActor(LiveActor*, const JMapInfoIter&);
    bool isRegisteredActor(const LiveActor*);
    MiniatureGalaxy* findMiniatureGalaxy(const char*) const;
    void killAllMiniatureGalaxy();
    s32 calcIndex(const LiveActor*) const;
#ifdef TARGET_KRP
    krp::ByamlIter getOrbitParams(s32 idx);
#endif
    void updateCometStatus();

    /* 0x8C */ LiveActorGroup* mMiniatureGalaxyGroup;
    /* 0x90 */ bool _90;
    /* 0x94 */ MiniatureGalaxy* mCometGalaxy;
    /* 0x98 */ s32 mCometID;
    /* 0x9C */ s32 _9C;
#ifdef TARGET_KRP
    /* 0x100 */ krp::ByamlIter mParamTable;
#endif
};

// TODO: cleanup

class MiniatureGalaxyFunction {
public:
#ifdef TARGET_KRP
    static void createHolder();
    static MiniatureGalaxyHolder* getHolder();
    static krp::ByamlIter getMiniatureGalaxyOrbitParams(s32 idx);
#else
    static void registerMiniatureGalaxyToHolder(LiveActor*, const JMapInfoIter&);
#endif
    static s32 getMiniatureGalaxyNum();
    static s32 calcMiniatureGalaxyIndex(const LiveActor*);
    static void updateCometStatus();
    static MiniatureGalaxy* getCometLandMiniatureGalaxy();
    static s32 getCometNameId();
    static MiniatureGalaxy* getPointingMiniatureGalaxy();
};
