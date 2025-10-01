#ifndef IG20241214180126
#define IG20241214180126

//* DEPRECATED

#include "TileData.h"
#include "raylibEx.h"
#include <vector>

namespace MovementData
{
    //* Movementspeed in tiles per second
    int constexpr MOVEMENT_SPEED{ 20 };
}

struct MovementComponent
{
    std::vector<Vector2I> path{};

    float baseSpeed{ MovementData::MOVEMENT_SPEED * TileData::TILE_SIZE };
};

#endif
