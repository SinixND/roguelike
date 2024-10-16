#include "CollisionSystem.h"

#include "Map.h"
#include "PositionComponent.h"
#include "raylibEx.h"

bool CollisionSystem::checkCollision(
    Map const& map,
    Vector2I const& tilePositionToCheck,
    Vector2I const& heroPosition)
{
    return (
        //* Next tilePosition unit moves to
        map.tiles.getIsSolids().contains(tilePositionToCheck)
        // || map.objects_.getIsSolids().contains(tilePositionToCheck)
        || map.enemies.getPositions().contains(PositionComponent{tilePositionToCheck})
        || Vector2Equals(tilePositionToCheck, heroPosition));
}
