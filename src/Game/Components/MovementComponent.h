#ifndef IG20241214180126
#define IG20241214180126

#include "TileData.h"
#include "raylibEx.h"
#include <vector>

//* Movementspeed in tiles per second
constexpr int MOVEMENTSPEED{ 20 };

struct MovementComponent
{
    std::vector<Vector2I> path{};

    float baseSpeed{ MOVEMENTSPEED * TileData::tileSize };
};

#endif
