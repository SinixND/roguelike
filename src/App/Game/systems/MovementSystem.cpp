#include "MovementSystem.h"

#include "Convert.h"
#include "Enemies.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "Hero.h"
#include "MovementComponent.h"
#include "raylibEx.h"
#include <cassert>
#include <cstddef>

[[nodiscard]]
Vector2 const& updateEntity(
    Vector2& position,
    MovementComponent& moveIO,
    float dt
)
{
    assert( moveIO.remainingDistance > 0 && "Movement done but not cleaned up" );

    //* TODO: Where/How to store/use speed? Separate component? Attributes?
    //* Find goblin.speedBase and replace accordingly
    if ( !moveIO.speed )
    {
        return position;
    }

    float frameDistance{ moveIO.speed * dt };

    //* Check if movement exceeds remaining distance this frame
    if ( frameDistance < moveIO.remainingDistance )
    {
        //* Move full distance this frame
        position += Vector2Scale(
            Vector2Normalize( moveIO.direction ),
            frameDistance
        );

        moveIO.remainingDistance -= frameDistance;
    }
    else
    {
        //* Move by remaining distance
        position += Vector2Scale(
            moveIO.direction,
            moveIO.remainingDistance
        );

        moveIO.remainingDistance = 0;

        //* === Moved one tile ===
        //* Clean precision errors
        position = Vector2Round( position );
    }

    return position;
}

namespace MovementSystem
{
    void update(
        Hero& heroIO,
        Enemies& enemiesIO,
        float dt
    )
    {
        bool allMovesDone{ true };

        if ( heroIO.movement )
        {
            allMovesDone = false;

            Vector2I oldPosition{ Convert::worldToTile( heroIO.position ) };

            heroIO.position = updateEntity(
                heroIO.position,
                *heroIO.movement,
                dt
            );

            snx::EventDispatcher::notify( EventId::HERO_MOVED );

            if ( oldPosition != Convert::worldToTile( heroIO.position ) )
            {
                snx::EventDispatcher::notify( EventId::HERO_POSITION_CHANGED );
            }

            if ( !heroIO.movement->remainingDistance )
            {
                heroIO.movement.reset();
            }
        }

        for ( size_t idx{ 0 }; idx < enemiesIO.moves.size(); ++idx )
        {
            allMovesDone = false;

            size_t enemyId{ enemiesIO.moves.key( idx ) };

            enemiesIO.positions.at( enemyId ) = updateEntity(
                enemiesIO.positions.at( enemyId ),
                enemiesIO.moves.at( enemyId ),
                dt
            );

            if ( !enemiesIO.moves.at( enemyId ).remainingDistance )
            {
                enemiesIO.moves.erase( enemyId );
            }
        }

        if ( allMovesDone )
        {
            snx::EventDispatcher::notify( EventId::MULTIFRAME_ACTIONS_DONE );
        };
    }
}
