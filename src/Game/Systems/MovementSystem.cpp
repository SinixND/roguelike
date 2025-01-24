#include "MovementSystem.h"

#include "Convert.h"
#include "Enemies.h"
#include "EnergyComponent.h"
#include "EventId.h"
#include "MovementComponent.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "TransformComponent.h"
#include "raylibEx.h"
#include <raymath.h>

namespace MovementSystem
{
    void update(
        TransformComponent& transform,
        // MovementComponent& movement,
        Vector2& position,
        EnergyComponent& energy,
        Vector2 const& heroPosition,
        float dt
    )
    {
        //* Check if action is in progress
        if ( transform.speed )
        {
            EnergyModule::consume( energy );

            snx::PublisherStatic::publish( EventId::MULTIFRAME_ACTION_ACTIVE );
        }
        else
        {
            return;
        }

        Vector2 offset{ frameOffset( transform, dt ) };

        transform.cumulativeDistance += Vector2Length( offset );

        Vector2I oldPosition{ Convert::worldToTile( position ) };

        //* Check if movement exceeds tile length this frame
        if ( transform.cumulativeDistance < TileData::tileSize )
        {
            //* Move full distance this frame
            position += offset;
        }
        else
        {
            //* Move by remaining distance until TILE_SIZE
            position += Vector2ClampValue(
                offset,
                0,
                TileData::tileSize
                    - ( transform.cumulativeDistance
                        - Vector2Length( offset ) )
            );

            //* === Moved one tile ===
            //* Clean precision errors
            position = Vector2Round( position );

            //* Reset cumulativeDistance
            resetCumulativeDistance( transform );

            resetTransform( transform );

            snx::PublisherStatic::publish( EventId::MULTIFRAME_ACTION_DONE );
        }

        //* Check if unit moving is the hero
        if ( position != heroPosition )
        {
            return;
        }

        //* Handle special case for hero
        snx::PublisherStatic::publish( EventId::HERO_MOVED );

        if ( oldPosition != Convert::worldToTile( position ) )
        {
            snx::PublisherStatic::publish( EventId::HERO_POSITION_CHANGED );
        }
    }

    void updateEnemies(
        Enemies& enemies,
        Vector2 const& heroPosition,
        float dt
    )
    {
        Vector2* currentPosition{};
        Vector2I oldPosition{};

        //* get new positions

        //* update all positions/movements/transforms

        for ( size_t idx{ 0 }; idx < enemies.transforms.size(); ++idx )
        {
            currentPosition = &enemies.positions.values().at( idx );

            oldPosition = Convert::worldToTile( *currentPosition );

            //* Update movement
            //* Update ids_ key if tilePosition changes
            MovementSystem::update(
                enemies.transforms.values().at( idx ),
                // enemies.movements.values().at(i),
                *currentPosition,
                enemies.energies.values().at( idx ),
                heroPosition,
                dt
            );

            if ( oldPosition != Convert::worldToTile( *currentPosition ) )
            {
                enemies.ids.changeKey(
                    oldPosition,
                    Convert::worldToTile( *currentPosition )
                );
            }
        }
    }

    void prepareByDirection(
        MovementComponent const& movement,
        Vector2I const& direction,
        TransformComponent& transform
    )
    {
        transform.direction = direction;
        transform.speed = movement.baseSpeed;
    }

    void prepareFromExistingPath(
        MovementComponent& movement,
        TransformComponent& transform
    )
    {
        prepareByDirection(
            movement,
            Vector2Subtract(
                movement.path.rbegin()[1],
                movement.path.rbegin()[0]
            ),
            transform
        );

        //* Remove tilePosition moved from
        movement.path.pop_back();

        //* Clear path guided movement after last trigger if target reched (path_.size() == 1 (target))
        if ( movement.path.size() <= 1 )
        {
            movement.path.clear();
        }
    }

    void prepareByNewPath(
        MovementComponent& movement,
        std::vector<Vector2I> const& path
    )
    {
        if ( path.empty() )
        {
            return;
        }

        movement.path = path;
    }

    void prepareByFromTo(
        MovementComponent& movement,
        TransformComponent& transform,
        Vector2I const& from,
        Vector2I const& to
    )
    {
        prepareByDirection(
            movement,
            Vector2Subtract(
                to,
                from
            ),
            transform
        );
    }

    void resetTransform( TransformComponent& transform )
    {
        // transform.isInProgress_ = false;
        transform.direction = Vector2I{ 0, 0 };
        transform.speed = .0f;
    }

    void resetCumulativeDistance( TransformComponent& transform )
    {
        transform.cumulativeDistance = 0;
    }

    Vector2 frameOffset(
        TransformComponent const& transform,
        float dt
    )
    {
        return Vector2Scale(
            transform.direction,
            transform.speed * dt
        );
    }
}
