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
    for ( size_t idx{ 0 }; idx < enemies.moves.size(); ++idx )
    {
        if ( Vector2Equals(
                 tilePositionToCheck,
                 Vector2Add(
                     Convert::worldToTile(
                         enemies.positions[idx]
                     ),
                     //* TODO: CHANGE/REMOVE
                     // enemies.transforms.values()[idx].direction
                     enemies.moves.values()[idx].direction
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
            tiles.isSolids.contains( tiles.ids.at( tilePositionToCheck ) )
            || enemies.ids.contains( tilePositionToCheck )
            //* Next tilePosition unit moves to
            || checkCollisionWithNextPosition(
                enemies,
                tilePositionToCheck
            )
            // || map.objects_.getIsSolids().contains(tilePositionToCheck)
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
            tiles.isSolids.contains( tiles.ids.at( tilePositionToCheck ) )
            || enemies.ids.contains( tilePositionToCheck )
            //* Next tilePosition unit moves to
            || checkCollisionWithNextPosition(
                enemies,
                tilePositionToCheck
            )
            // || map.objects_.getIsSolids().contains(tilePositionToCheck)
            || Vector2Equals( tilePositionToCheck, heroPosition )
        );
    }
}
