#include "Collision.h"

#include "Map.h"
#include "raylibEx.h"

bool Collision::checkCollision(
    Map const& map,
    Vector2I const& positionToCheck,
    Vector2I const& heroPosition)
{
    return (
        // Next tilePosition unit moves to
        map.tiles_.isSolid(positionToCheck)
        || map.enemies_.ids().contains(positionToCheck)
        || Vector2Equals(positionToCheck, heroPosition));
}