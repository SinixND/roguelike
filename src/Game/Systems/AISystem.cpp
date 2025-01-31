#include "AISystem.h"

#include "CollisionSystem.h"
#include "Convert.h"
#include "DamageSystem.h"
#include "Enemies.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include <cstddef>
#include <vector>

bool executeAction(
    Enemies& enemiesIO,
    HealthComponent& heroHealthIO,
    Map const& map,
    Vector2I const& heroPosition,
    GameCamera const& gameCamera,
    size_t enemyId
)
{
    bool isActionMultiFrame{ false };

    //* Instant action: attack
    if ( Vector2Length(
             Vector2Subtract(
                 Convert::worldToTile( enemiesIO.positions[enemyId] ),
                 heroPosition
             )
         )
         == 1 )
    {
        //* Attack
        snx::Logger::log( "Hero takes " );

        DamageSystem::attack(
            enemiesIO.damages[enemyId],
            heroHealthIO
        );

        EnergyModule::consume( enemiesIO.energies[enemyId] );
    }
    //* Check path
    else
    {
        std::vector<Vector2I> path{ PathfinderSystem::findPath(
            map,
            Convert::worldToTile( enemiesIO.positions[enemyId] ),
            heroPosition,
            gameCamera,
            false,
            enemiesIO.ais[enemyId].scanRange
        ) };

        size_t pathSize{ path.size() };

        //* Path is valid: has at least 2 entries (start and target)
        //* Move
        if ( pathSize > 2
             && !CollisionSystem::checkCollision(
                 map.tiles,
                 map.enemies,
                 path.rbegin()[1],
                 heroPosition
             ) )
        {
            MovementSystem::prepareByFromTo(
                enemiesIO.movements[enemyId],
                enemiesIO.transforms[enemyId],
                Convert::worldToTile( enemiesIO.positions[enemyId] ),
                path.rbegin()[1]
            );

            EnergyModule::consume( enemiesIO.energies[enemyId] );

            isActionMultiFrame = true;
        }
        //* Path is "empty"
        //* Wait
        else
        {
            //* Wait
            EnergyModule::consume( enemiesIO.energies[enemyId] );
        }
    }
    // return false;
    return isActionMultiFrame;
}

namespace AISystem
{
    bool executeNextAction(
        size_t& activeEnemyIdIO,
        Enemies& enemiesIO,
        Hero& heroIO,
        Map const& map,
        GameCamera const& gameCamera,
        int turn
    )
    {
        bool multiFrameActionActive{ false };

        do
        {
            activeEnemyIdIO = EnemiesModule::getActive(
                enemiesIO.energies,
                enemiesIO.ais,
                turn
            );

            if ( activeEnemyIdIO )
            {
                enemiesIO.ais[activeEnemyIdIO].turn = turn;

                multiFrameActionActive |= executeAction(
                    enemiesIO,
                    heroIO.health,
                    map,
                    Convert::worldToTile( heroIO.position ),
                    gameCamera,
                    activeEnemyIdIO
                );
            }
        } while ( activeEnemyIdIO );

        return multiFrameActionActive;
    }
}
