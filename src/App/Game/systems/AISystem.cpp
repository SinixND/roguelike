#include "AISystem.h"

// #define DEBUG_AI_ACTIONS

#include "ActionId.h"
#include "CollisionSystem.h"
#include "CombatSystem.h"
#include "Convert.h"
#include "Enemies.h"
#include "Hero.h"
#include "Map.h"
#include "MoveComponent.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include <cstddef>
#include <vector>

#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
#include "Debugger.h"
#endif

[[nodiscard]]
Enemies const& handleEmptyPath(
    Enemies& enemies,
    size_t enemyId
)
{
    //* Wait
    enemies.actions.insert(
        enemyId,
        ActionId::WAIT
    );

    return enemies;
}

[[nodiscard]]
Enemies const& handleExisingPath(
    Enemies& enemies,
    size_t enemyId,
    std::vector<Vector2I> const& path,
    Map const& map,
    Vector2I const& target
)
{
    //* Attack
    if ( path.size() == 2 )
    {
#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
        snx::Debugger::cliLog( "Add attack component to hero.\n" );
#endif
        enemies.attacks.insert(
            enemyId,
            AttackComponent{ target }
        );

        return enemies;
    }

    //* Move
    else if ( map.tiles.isSolids.contains( map.tiles.ids.at( target ) ) )
    {
#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
        snx::Debugger::cliLog( "Add move component to hero\n" );
#endif
        enemies.moves.insert(
            enemyId,
            MoveComponent{ target }
        );

        enemies.paths.erase( enemyId );
        enemies.paths.insert( enemyId, path );
        enemies.paths.at( enemyId ).pop_back();
    }

    return enemies;
}

//* TODO: CHANGE/REMOVE
Enemies& executeAction(
    Enemies& enemiesIO,
    Hero& heroIO,
    Map const& map,
    GameCamera const& gameCamera,
    size_t enemyId
)
{
    //* Instant action: attack
    if ( Vector2Length(
             Vector2Subtract(
                 Convert::worldToTile( enemiesIO.positions.at( enemyId ) ),
                 Convert::worldToTile( heroIO.position )
             )
         )
         == 1 )
    {
        //* Attack
#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
        snx::Debugger::cliLog( "Enemy[", enemyId, "] attacks and deals" );
#endif
        CombatSystem::performAttack(
            enemiesIO.energies.at( enemyId ),
            enemiesIO.damages.at( enemyId ),
            heroIO.health
        );
    }
    //* Check path
    else
    {
        std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
            Convert::worldToTile( enemiesIO.positions.at( enemyId ) ),
            Convert::worldToTile( heroIO.position ),
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
                 Convert::worldToTile( heroIO.position )
             ) )
        {
#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
            snx::Debugger::cliLog( "Enemy[", enemyId, "] moves.\n" );
#endif
            enemiesIO.energies.at( enemyId ) = EnergyModule::exhaust( enemiesIO.energies.at( enemyId ) );

            enemiesIO.transforms.at( enemyId ) = MovementSystem::prepareByFromTo(
                enemiesIO.transforms.at( enemyId ),
                enemiesIO.movements.at( enemyId ),
                Convert::worldToTile( enemiesIO.positions.at( enemyId ) ),
                path.rbegin()[1]
            );
        }
        //* Path is "empty" or own position
        //* Wait
        else
        {
            //* Wait
#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
            snx::Debugger::cliLog( "Enemy[", enemyId, "] waits.\n" );
#endif
            enemiesIO.energies.at( enemyId ) = EnergyModule::exhaust( enemiesIO.energies.at( enemyId ) );
        }
    }
    return enemiesIO;
}

namespace AISystem
{
    void update(
        Enemies& enemiesIO,
        Map const& map,
        Hero const& hero,
        GameCamera const& gameCamera
    )
    {
        for ( size_t idx{ 0 }; idx < enemiesIO.isReadies.size(); ++idx )
        {
            size_t enemyId{ enemiesIO.isReadies.key( idx ) };

            std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
                Convert::worldToTile( enemiesIO.positions.at( enemyId ) ),
                Convert::worldToTile( hero.position ),
                map,
                gameCamera,
                false,
                enemiesIO.ais.at( enemyId ).scanRange
            ) };

            size_t pathSize{ path.size() };

            if ( !pathSize )
            {
                enemiesIO = handleEmptyPath(
                    enemiesIO,
                    enemyId
                );
            }

            else
            {
                enemiesIO = handleExisingPath(
                    enemiesIO,
                    enemyId,
                    path,
                    map,
                    Convert::worldToTile( hero.position )
                );
            }
        }
    }

    //* TODO: CHANGE/REMOVE
    Enemies const& executeNextAction(
        Enemies& enemies,
        size_t& activeEnemyIdIO,
        Hero& heroIO,
        Map const& map,
        GameCamera const& gameCamera
    )
    {
        do
        {
            activeEnemyIdIO = EnemiesModule::getActive( enemies.energies );

            if ( activeEnemyIdIO )
            {
                enemies = executeAction(
                    enemies,
                    heroIO,
                    map,
                    gameCamera,
                    activeEnemyIdIO
                );
            }
        } while ( activeEnemyIdIO );

        return enemies;
    }
}
