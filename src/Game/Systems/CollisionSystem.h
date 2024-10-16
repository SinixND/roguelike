#ifndef IG20240914212555
#define IG20240914212555

#include "Map.h"
#include "raylibEx.h"

namespace CollisionSystem
{
    //* Checks for
    //* - solid tiles
    //* - enemies
    //* Returns if collision occurs
    bool checkCollision(
        Map const& map,
        Vector2I const& tilePositionToCheck,
        Vector2I const& heroPosition);
}

#endif