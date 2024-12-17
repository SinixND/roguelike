#include "CollisionSystem.h"

#include "Enemies.h"
#include "Tiles.h"
#include "raylibEx.h"

bool CollisionSystem::checkCollision(
    Tiles const& tiles,
    Enemies const& enemies,
    // Objects const& objects,
    Vector2I const& tilePositionToCheck,
    Vector2I const& heroPosition)
{
    return (
        //* Next tilePosition unit moves to
        enemies.ids.contains(tilePositionToCheck)
        // || map.objects_.getIsSolids().contains(tilePositionToCheck)
        || tiles.isSolids.contains(tilePositionToCheck)
        || Vector2Equals(tilePositionToCheck, heroPosition));
}
