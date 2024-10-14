#include "Collision.h"

#include "Map.h"
#include "Position.h"
#include "raylibEx.h"

bool Collision::checkCollision(
    Map const& map,
    Vector2I const& tilePositionToCheck,
    Vector2I const& heroPosition)
{
    return (
        // Next tilePosition unit moves to
        map.tiles_.isSolid(tilePositionToCheck)
        // || map.objects_.isSolid(tilePositionToCheck)
        || map.enemies_.positions().contains(Position{tilePositionToCheck})
        || Vector2Equals(tilePositionToCheck, heroPosition));
}
