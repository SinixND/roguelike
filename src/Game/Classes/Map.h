#ifndef IG20240907173344
#define IG20240907173344

#include "EnemySoA.h"
#include "ObjectSoA.h"
#include "TileSoA.h"

struct Map
{
    TileSoA tiles{};
    ObjectSoA objects{};
    EnemySoA enemies{};
};

#endif
