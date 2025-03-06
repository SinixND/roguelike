#include "AIComponent.h"

#include "CollisionSystem.h"
#include "Convert.h"
#include "Debugger.h"
#include "Enemies.h"
#include "HealthComponent.h"
#include "Hero.h"
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
                 Convert::worldToTile( enemiesIO.positions.at( enemyId ) ),
                 heroPosition
             )
         )
         == 1 )
    {
        //* Attack
#if defined( DEBUG )
        snx::debug::cliLog( "Enemy[", enemyId, "] attacks and deals" );
#endif
        EnergyModule::consume( enemiesIO.energies.at( enemyId ) );

        HealthModule::damage(
            heroHealthIO,
            DamageModule::damageRNG( enemiesIO.damages.at( enemyId ) )
        );
    }
    //* Check path
    else
    {
        std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
            Convert::worldToTile( enemiesIO.positions.at( enemyId ) ),
            heroPosition,
            map,
            gameCamera,
            false,
            enemiesIO.ais.at( enemyId ).scanRange
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
#if defined( DEBUG )
            snx::debug::cliLog( "Enemy[", enemyId, "] moves.\n" );
#endif
            EnergyModule::consume( enemiesIO.energies.at( enemyId ) );

            enemiesIO.transforms.at( enemyId ) = MovementSystem::prepareByFromTo(
                enemiesIO.transforms.at( enemyId ),
                enemiesIO.movements.at( enemyId ),
                Convert::worldToTile( enemiesIO.positions.at( enemyId ) ),
                path.rbegin()[1]
            );
        }
        //* Path is "empty"
        //* Wait
        else
        {
            //* Wait
#if defined( DEBUG )
            snx::debug::cliLog( "Enemy[", enemyId, "] waits.\n" );
#endif
            EnergyModule::consume( enemiesIO.energies.at( enemyId ) );
        }
    }
    return enemiesIO;
}

namespace AIModule
{
    Enemies const& executeNextAction(
        Enemies& enemies,
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
                enemies.energies,
                enemies.ais,
                turn
            );

            if ( activeEnemyIdIO )
            {
                enemies.ais.at( activeEnemyIdIO ).turn = turn;

                enemies = executeAction(
                    enemies,
                    heroIO.health,
                    map,
                    Convert::worldToTile( heroIO.position ),
                    gameCamera,
                    activeEnemyIdIO
                );
            }
        } while ( activeEnemyIdIO );

        return enemies;
    }
}
