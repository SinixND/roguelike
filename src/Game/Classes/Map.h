#ifndef IG20240907173344
#define IG20240907173344

#include "Enemies.h"
#include "Fog.h"
#include "Objects.h"
#include "Tiles.h"

struct Map
{
    Tiles tiles{};
    Objects objects{};
    Enemies enemies{};
    snx::DenseMap<Vector2I, Fog> fogs{};
};

#endif
