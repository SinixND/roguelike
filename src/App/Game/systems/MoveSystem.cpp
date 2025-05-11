#include "MoveSystem.h"

#include "Enemies.h"
#include "Hero.h"
#include "Logger.h"
#include "raylibEx.h"
#include <string>

[[nodiscard]]
Vector2 const& updateEntity(
    Vector2& position,
    MoveComponent& moveIO,
    float dt
)
{
    //* TODO: Where/How to store/use speed? Separate component? Attributes?
    //* TODO: Find goblin.speedBase and replace accordingly
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

        moveIO.speed = 0;

        //* === Moved one tile ===
        //* Clean precision errors
        position = Vector2Round( position );
    }

    return position;
}

namespace MoveSystem
{
    bool update(
        Hero& heroIO,
        Enemies& enemiesIO,
        float dt
    )
    {
        bool allMovesDone{ true };

        if ( heroIO.move )
        {
            allMovesDone = false;

            heroIO.energy = EnergyModule::exhaust( heroIO.energy );

            heroIO.position = updateEntity(
                heroIO.position,
                *heroIO.move,
                dt
            );

            if ( !heroIO.move->speed )
            {
                heroIO.move.reset();
            }
        }

        for ( size_t idx{ 0 }; idx < enemiesIO.moves.size(); ++idx )
        {
            allMovesDone = false;

            size_t enemyId{ enemiesIO.moves.key( idx ) };

            enemiesIO.energies.at( enemyId ) = EnergyModule::exhaust( enemiesIO.energies.at( enemyId ) );

            enemiesIO.positions.at( enemyId ) = updateEntity(
                enemiesIO.positions.at( enemyId ),
                enemiesIO.moves.at( enemyId ),
                dt
            );

            if ( !enemiesIO.moves.at( enemyId ).speed )
            {
                enemiesIO.moves.erase( enemyId );
            }
        }

        return allMovesDone;
    }
}
