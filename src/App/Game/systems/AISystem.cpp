#include "AISystem.h"

// #define DEBUG_AI_ACTIONS

#include "ActionId.h"
#include "CollisionSystem.h"
#include "CombatSystem.h"
#include "Convert.h"
#include "Enemies.h"
#include "EnemiesData.h"
#include "Hero.h"
#include "Map.h"
#include "MoveComponent.h"
#include "PathfinderSystem.h"
#include "TileData.h"
#include "raylibEx.h"
#include <cstddef>
#include <vector>

#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
#include "Debugger.h"
#endif

[[nodiscard]]
Enemies const& handleInvalidPath(
    Enemies& enemies,
    size_t enemyId
)
{
    //* Wait
    enemies.energies.at( enemyId ) = EnergyModule::exhaust( enemies.energies.at( enemyId ) );

    enemies.actions.insert(
        enemyId,
        ActionId::WAIT
    );

    return enemies;
}

[[nodiscard]]
Enemies const& handleValidPath(
    Enemies& enemies,
    Vector2I const& heroPosition,
    size_t enemyId,
    Vector2I const& adjacentPathTile,
    Map const& map
)
{
    //* Attack
    if ( heroPosition == adjacentPathTile )
    {
#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
        snx::Debugger::cliLog( "Add attack component to hero.\n" );
#endif
        enemies.energies.at( enemyId ) = EnergyModule::exhaust( enemies.energies.at( enemyId ) );

        enemies.attacks.insert(
            enemyId,
            AttackComponent{ adjacentPathTile }
        );

        return enemies;
    }

    //* Move
    else if ( !CollisionSystem::checkCollisionForEnemy(
                  map.tiles,
                  enemies,
                  adjacentPathTile,
                  heroPosition
              ) )
    {
#if defined( DEBUG ) && defined( DEBUG_AI_ACTIONS )
        snx::Debugger::cliLog( "Add move component to enemy\n" );
#endif
        Vector2I direction{ Vector2MainDirection(
            Convert::worldToTile(
                enemies.positions.at( enemyId )
            ),
            adjacentPathTile
        ) };

        enemies.moves.insert(
            enemyId,
            MoveComponent{
                direction,
                EnemiesData::goblin.speedBase,
                TileData::TILE_SIZE
            }
        );

        Vector2I currentPosition{
            Convert::worldToTile( enemies.positions.at( enemyId ) )
        };

        enemies.ids.changeKey(
            currentPosition,
            Vector2Add( currentPosition, direction )
        );
    }

    enemies.energies.at( enemyId ) = EnergyModule::exhaust( enemies.energies.at( enemyId ) );

    return enemies;
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
        for ( size_t idx{ 0 }; idx < enemiesIO.isIdles.size(); ++idx )
        {
            size_t enemyId{ enemiesIO.isIdles.key( idx ) };

            std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
                Convert::worldToTile( enemiesIO.positions.at( enemyId ) ),
                Convert::worldToTile( hero.position ),
                map,
                gameCamera,
                false,
                enemiesIO.ais.at( enemyId ).scanRange
            ) };

            if ( path.empty() )
            {
                enemiesIO = handleInvalidPath(
                    enemiesIO,
                    enemyId
                );
            }

            else
            {
                enemiesIO = handleValidPath(
                    enemiesIO,
                    Convert::worldToTile( hero.position ),
                    enemyId,
                    path.rbegin()[1],
                    map
                );
            }
        }

        enemiesIO.isIdles.clear();
    }
}
