#include "Collision.h"

#include "Map.h"
#include "raylibEx.h"

bool Collision::checkCollision(Map const& map, Vector2I const& positionToCheck)
{
    return (map.tiles_.isSolid(
                // Next tilePosition hero moves to
                positionToCheck)
            || map.enemies_.ids().contains(positionToCheck));
}