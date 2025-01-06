#include "CollisionSystem.h"

#include "Convert.h"
#include "Enemies.h"
#include "Tiles.h"
#include "raylibEx.h"

bool checkCollisionWithNextPosition(
    Enemies const& enemies,
    Vector2I const& tilePositionToCheck)
{
    for (size_t idx{0}; idx < enemies.transforms.size(); ++idx)
    {
        if (Vector2Equals(
                tilePositionToCheck,
                Vector2Add(
                    Convert::worldToTile(enemies.positions.values().at(idx)),
                    enemies.transforms.values().at(idx).direction)))
        {
            return true;
        }
    }

    return false;
}

bool CollisionSystem::checkCollision(
    Tiles const& tiles,
    Enemies const& enemies,
    // Objects const& objects,
    Vector2I const& tilePositionToCheck,
    Vector2I const& heroPosition)
{
    size_t enemyId{enemies.ids.contains(tilePositionToCheck)};
    size_t tileId{tiles.ids.contains(tilePositionToCheck)};

    return (
        //* Next tilePosition unit moves to
        enemyId
        // || map.objects_.getIsSolids().contains(tilePositionToCheck)
        || tiles.isSolids.contains(tileId)
        || Vector2Equals(tilePositionToCheck, heroPosition)
        || checkCollisionWithNextPosition(
            enemies,
            tilePositionToCheck));
}
