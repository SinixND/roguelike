#ifndef IG20240531201830
#define IG20240531201830

#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>

struct TransformComponent
{
    Vector2I direction{};
    float speed{};
    float remainingDistance{ TileData::tileSize };
};

#endif
