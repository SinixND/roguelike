#include "MovementSystem.h"

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
    Vector2 const& updateSingle(
        Vector2& position,
        TransformComponent& transformIO,
        // MovementComponent& movementIO,
        EnergyComponent& energyIO,
        float dt
    )
    {
        EnergyModule::consume( energyIO );

        Vector2 offset{ frameOffset( transformIO, dt ) };

        transformIO.cumulativeDistance += Vector2Length( offset );

        //* Check if movement exceeds tile length this frame
        if ( transformIO.cumulativeDistance < TileData::tileSize )
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
                    - ( transformIO.cumulativeDistance
                        - Vector2Length( offset ) )
            );

            //* === Moved one tile ===
            //* Clean precision errors
            position = Vector2Round( position );

            //* Reset cumulativeDistance
            transformIO = resetTransform( transformIO );

            snx::PublisherStatic::publish( EventId::MULTIFRAME_ACTION_DONE );
        }

        return position;
    }

    TransformComponent const& prepareByDirection(
        TransformComponent& transform,
        MovementComponent const& movement,
        Vector2I const& direction
    )
    {
        transform.direction = direction;
        transform.speed = movement.baseSpeed;

        snx::PublisherStatic::publish( EventId::MULTIFRAME_ACTION_ACTIVE );

        return transform;
    }

    MovementComponent const& prepareByNewPath(
        MovementComponent& movement,
        std::vector<Vector2I> const& path
    )
    {
        if ( path.empty() )
        {
            return movement;
        }

        movement.path = path;

        return movement;
    }

    TransformComponent const& prepareFromExistingPath(
        TransformComponent& transform,
        MovementComponent& movementIO
    )
    {
        transform = prepareByDirection(
            transform,
            movementIO,
            Vector2Subtract(
                movementIO.path.rbegin()[1],
                movementIO.path.rbegin()[0]
            )
        );

        //* Remove tilePosition moved from
        movementIO.path.pop_back();

        //* Clear path guided movement after last trigger if target reched
        if ( movementIO.path.size() < 2 )
        {
            movementIO.path.clear();
        }

        return transform;
    }

    TransformComponent const& prepareByFromTo(
        TransformComponent& transform,
        MovementComponent const& movement,
        Vector2I const& from,
        Vector2I const& to
    )
    {
        transform = prepareByDirection(
            transform,
            movement,
            Vector2Subtract(
                to,
                from
            )
        );

        return transform;
    }

    TransformComponent const& resetTransform( TransformComponent& transform )
    {
        // transform.isInProgress_ = false;
        transform.direction = Vector2I{ 0, 0 };
        transform.speed = .0f;
        transform.cumulativeDistance = 0;

        snx::PublisherStatic::publish( EventId::MULTIFRAME_ACTION_DONE );

        return transform;
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
