#include "CollisionSystem.h"
#include "Map.h"
#include "raylibEx.h"

bool CollisionSystem::checkCollision(
    Map const& map,
    Vector2I const& tilePositionToCheck,
    Vector2I const& heroPosition)
{
    return (
        //* Next tilePosition unit moves to
        map.enemies.ids.contains(tilePositionToCheck)
        // || map.objects_.getIsSolids().contains(tilePositionToCheck)
        // || map.tiles.isSolids.contains(tilePositionToCheck)
        || Vector2Equals(tilePositionToCheck, heroPosition));
}
