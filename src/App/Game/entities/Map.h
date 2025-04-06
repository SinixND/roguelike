#ifndef IG20240907173344
#define IG20240907173344

#include "DenseMap.h"
#include "Enemies.h"
#include "FogStateId.h"
#include "Objects.h"
#include "Tiles.h"

struct Map
{
    Enemies enemies{};
    Objects objects{};
    Tiles tiles{};
    snx::DenseMap<Vector2I, FogStateId> fogs{};
};

#endif
