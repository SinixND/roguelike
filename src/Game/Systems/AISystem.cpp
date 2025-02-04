#include "AISystem.h"

#include "CollisionSystem.h"
#include "Convert.h"
#include "Enemies.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include <cstddef>
#include <vector>

Enemies& executeAction(
    Enemies& enemiesIO,
    HealthComponent& heroHealthIO,
    Map const& map,
    Vector2I const& heroPosition,
    GameCamera const& gameCamera,
    size_t enemyId
)
{
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

        HealthModule::damage(
            heroHealthIO,
            DamageModule::damageRNG( enemiesIO.damages[enemyId] )
        );

        EnergyModule::consume( enemiesIO.energies[enemyId] );
    }
    //* Check path
    else
    {
        std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
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
            enemiesIO.transforms[enemyId] = MovementSystem::prepareByFromTo(
                enemiesIO.transforms[enemyId],
                enemiesIO.movements[enemyId],
                Convert::worldToTile( enemiesIO.positions[enemyId] ),
                path.rbegin()[1]
            );

            EnergyModule::consume( enemiesIO.energies[enemyId] );
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
    return enemiesIO;
}

namespace AISystem
{
    Enemies const& executeNextAction(
        Enemies& enemiesIO,
        size_t& activeEnemyIdIO,
        Hero& heroIO,
        Map const& map,
        GameCamera const& gameCamera,
        int turn
    )
    {
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

                enemiesIO = executeAction(
                    enemiesIO,
                    heroIO.health,
                    map,
                    Convert::worldToTile( heroIO.position ),
                    gameCamera,
                    activeEnemyIdIO
                );
            }
        } while ( activeEnemyIdIO );

        return enemiesIO;
    }
}
