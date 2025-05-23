#include "CollisionSystem.h"

#include "Convert.h"
#include "Enemies.h"
#include "Tiles.h"
#include "raylibEx.h"

bool checkCollisionWithNextPosition(
    Enemies const& enemies,
    Vector2I const& tilePositionToCheck
)
{
    for ( size_t idx{ 0 }; idx < enemies.transforms.size(); ++idx )
    {
        if ( Vector2Equals(
                 tilePositionToCheck,
                 Vector2Add(
                     Convert::worldToTile(
                         enemies.positions[idx]
                     ),
                     enemies.transforms.values()[idx].direction
                 )
             ) )
        {
            return true;
        }
    }

    return false;
}

namespace CollisionSystem
{
    bool checkCollisionForHero(
        Tiles const& tiles,
        Enemies const& enemies,
        // Objects const& objects,
        Vector2I const& tilePositionToCheck
    )
    {
        return (
            //* Next tilePosition unit moves to
            enemies.ids.contains( tilePositionToCheck )
            // || map.objects_.getIsSolids().contains(tilePositionToCheck)
            || tiles.isSolids.contains( tiles.ids.at( tilePositionToCheck ) )
            || checkCollisionWithNextPosition(
                enemies,
                tilePositionToCheck
            )
        );
    }
    bool checkCollisionForEnemy(
        Tiles const& tiles,
        Enemies const& enemies,
        // Objects const& objects,
        Vector2I const& tilePositionToCheck,
        Vector2I const& heroPosition
    )
    {
        return (
            //* Next tilePosition unit moves to
            enemies.ids.contains( tilePositionToCheck )
            // || map.objects_.getIsSolids().contains(tilePositionToCheck)
            || tiles.isSolids.contains( tiles.ids.at( tilePositionToCheck ) )
            || Vector2Equals( tilePositionToCheck, heroPosition )
            || checkCollisionWithNextPosition(
                enemies,
                tilePositionToCheck
            )
        );
    }
}
