#include "Collision.h"

#include "Map.h"
#include "raylibEx.h"

bool Collision::checkCollision(Map& map, Vector2I const& positionToCheck)
{
    return (map.tiles().isSolid(
                // Next tilePosition hero moves to
                positionToCheck)
            || map.enemies().ids().contains(positionToCheck));
}