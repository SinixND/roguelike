#include "AISystem.h"

#include "CollisionSystem.h"
#include "Convert.h"
#include "DamageComponent.h"
#include "DamageSystem.h"
#include "Enemies.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include "TransformComponent.h"
#include "World.h"
#include <cstddef>
#include <vector>

namespace AISystem
{
    bool takeActions(
        World& world,
        Hero& hero,
        size_t& activeEnemyId,
        GameCamera const& gameCamera,
        int turn
    )
    {
        bool multiFrameActionActive{ false };

        Enemies& enemies{ world.currentMapPtr->enemies };

        do
        {
            activeEnemyId = EnemiesModule::getActive(
                enemies.energies,
                enemies.ais,
                turn
            );

            if ( activeEnemyId )
            {
                enemies.ais[activeEnemyId].turn = turn;

                multiFrameActionActive |= AISystem::takeAction(
                    enemies.ais[activeEnemyId],
                    enemies.positions[activeEnemyId],
                    enemies.movements[activeEnemyId],
                    enemies.transforms[activeEnemyId],
                    enemies.energies[activeEnemyId],
                    enemies.damages[activeEnemyId],
                    *world.currentMapPtr,
                    Convert::worldToTile( hero.position ),
                    hero.health,
                    gameCamera
                );
            }
        } while ( activeEnemyId );

        return multiFrameActionActive;
    }

    bool takeAction(
        AIComponent const& ai,
        Vector2& position,
        MovementComponent& movement,
        TransformComponent& transform,
        EnergyComponent& energy,
        DamageComponent& damage,
        Map const& map,
        Vector2I const& heroPosition,
        HealthComponent& heroHealth,
        GameCamera const& gameCamera
    )
    {
        bool isActionMultiFrame{ false };

        //* Instant action: attack
        if ( Vector2Length(
                 Vector2Subtract(
                     Convert::worldToTile( position ),
                     heroPosition
                 )
             )
             == 1 )
        {
            //* Attack
            snx::Logger::log( "Hero takes " );

            DamageSystem::attack(
                damage,
                heroHealth
            );

            EnergyModule::consume( &energy );
        }
        //* Check path
        else
        {
            std::vector<Vector2I> path{ PathfinderSystem::findPath(
                map,
                Convert::worldToTile( position ),
                heroPosition,
                gameCamera,
                false,
                ai.scanRange
            ) };

            size_t pathSize{ path.size() };

            //* Path is valid: has at least 2 entries (start and target) -> Multi-frame action: move
            //* TODO: Check all enemies->next position to avoid two enemies moving to the same spot
            if ( pathSize > 2
                 && !CollisionSystem::checkCollision(
                     map.tiles,
                     map.enemies,
                     path.rbegin()[1],
                     heroPosition
                 ) )
            {
                //* Prepare multi-frame action
                MovementSystem::prepareByFromTo(
                    movement,
                    transform,
                    Convert::worldToTile( position ),
                    path.rbegin()[1]
                );

                EnergyModule::consume( &energy );

                isActionMultiFrame = true;
            }
            //* Path is empty -> Instant action: wait
            else
            {
                //* Wait
                EnergyModule::consume( &energy );
            }
        }
        // return false;
        return isActionMultiFrame;
    }
}
