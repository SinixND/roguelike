#include "CollisionSystem.h"

#include "Enemies.h"
#include "PositionComponent.h"
#include "Tiles.h"
#include "raylibEx.h"

bool CollisionSystem::checkCollision(
    Tiles const& tiles,
    Enemies const& enemies,
    // Objects const& objects,
    Vector2I const& tilePositionToCheck,
    Vector2I const& heroPosition)
{
    size_t id{enemies.positions.contains(PositionComponent{tilePositionToCheck})};

    return (
        id
        // || map.objects_.getIsSolids().contains(tilePositionToCheck)
        || tiles.isSolids.at(id)
        || Vector2Equals(tilePositionToCheck, heroPosition));
}
