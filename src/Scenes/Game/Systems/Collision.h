#ifndef IG20240914212555
#define IG20240914212555

#include "Map.h"
#include "raylibEx.h"

namespace Collision
{
    // Checks for
    // - solid tiles
    // - enemies
    bool checkCollision(Map& map, Vector2I const& positionToCheck);
}

#endif