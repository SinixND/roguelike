#include "ActionSystem.h"

#include "CollisionSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Directions.h"
#include "EventDispatcher.h"
#include "GameCamera.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "InputId.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

#if defined( DEBUG )
#include "Debugger.h"
#endif

[[nodiscard]]
Hero const& processDirectionalInput(
    Hero& hero,
    Map& mapIO,
    Vector2I direction
)
{
    Vector2I target{
        Vector2Add(
            Convert::worldToTile( hero.position ),
            direction
        )
    };

    //* Attack
    if ( mapIO.enemies.ids.contains( target ) )
    {
#if defined( DEBUG )
        snx::debug::cliLog( "Hero attacks and deals " );
#endif
        EnergyModule::consume( hero.energy );

        HealthModule::damage(
            mapIO.enemies.healths[mapIO.enemies.ids.index( target )],
            DamageModule::damageRNG( hero.damage )
        );
    }
    //* Move
    else if ( !CollisionSystem::checkCollision(
                  mapIO.tiles,
                  mapIO.enemies,
                  target,
                  Convert::worldToTile( hero.position )
              ) )
    {
#if defined( DEBUG )
        snx::debug::cliLog( "Hero moves.\n" );
#endif
        EnergyModule::consume( hero.energy );

        hero.transform = MovementSystem::prepareByDirection(
            hero.transform,
            hero.movement,
            direction
        );
    }

    return hero;
}

namespace ActionSystem
{
    Hero const& executeAction(
        Hero& hero,
        Map& mapIO,
        Cursor const& cursor,
        GameCamera const& gameCamera,
        InputId currentInput
    )
    {
        switch ( currentInput )
        {
            default:
            case InputId::NONE:
            {
                //* Multi-frame action
                if ( !hero.movement.path.empty()
                     && !hero.transform.speed )
                {
                    //* Move
                    if ( !CollisionSystem::checkCollision(
                             mapIO.tiles,
                             mapIO.enemies,
                             hero.movement.path.rbegin()[1],
                             Convert::worldToTile( hero.position )
                         ) )
                    {
#if defined( DEBUG )
                        snx::debug::cliLog( "Hero moves.\n" );
#endif
                        EnergyModule::consume( hero.energy );

                        hero.transform = MovementSystem::prepareFromExistingPath(
                            hero.transform,
                            hero.movement
                        );
                    }
                    //* Stop
                    else
                    {
                        hero.transform = MovementSystem::resetTransform( hero.transform );
                        hero.movement.path.clear();
                    }
                }
                break;
            }

            case InputId::ACT_UP:
            {
                hero = processDirectionalInput(
                    hero,
                    mapIO,
                    Directions::up
                );

                break;
            }

            case InputId::ACT_LEFT:
            {
                hero = processDirectionalInput(
                    hero,
                    mapIO,
                    Directions::left
                );

                break;
            }

            case InputId::ACT_DOWN:
            {
                hero = processDirectionalInput(
                    hero,
                    mapIO,
                    Directions::down
                );

                break;
            }

            case InputId::ACT_RIGHT:
            {
                hero = processDirectionalInput(
                    hero,
                    mapIO,
                    Directions::right
                );

                break;
            }

            //* NOTE: Fall through possible to ACT_IN_PLACE if cursor.position == hero.position
            case InputId::ACT_TO_TARGET:
            {
                std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
                    Convert::worldToTile( hero.position ),
                    Convert::worldToTile( cursor.position ),
                    mapIO,
                    gameCamera
                ) };

                if (
                    Convert::worldToTile( hero.position )
                    == Convert::worldToTile( cursor.position )
                )
                {
                    //* NOTE: Fallthrough
                }
                else if ( path.empty() )
                {
                    break;
                }
                //* Check if an enemy is at the first path tile
                else if ( mapIO.enemies.ids.contains( path.rbegin()[1] ) )
                {
#if defined( DEBUG )
                    snx::debug::cliLog( "Hero attacks.\n" );
#endif
                    EnergyModule::consume( hero.energy );

                    snx::Logger::log( "Hero deals " );
                    HealthModule::damage(
                        mapIO.enemies.healths[mapIO.enemies.ids.index( path.rbegin()[1] )],
                        DamageModule::damageRNG( hero.damage )
                    );

                    path.clear();
                    break;
                }
                else if ( !CollisionSystem::checkCollision(
                              mapIO.tiles,
                              mapIO.enemies,
                              path.rbegin()[1],
                              Convert::worldToTile( hero.position )
                          ) )
                {
#if defined( DEBUG )
                    snx::debug::cliLog( "Hero moves.\n" );
#endif
                    EnergyModule::consume( hero.energy );

                    hero.movement = MovementSystem::prepareByNewPath(
                        hero.movement,
                        path
                    );

                    hero.transform = MovementSystem::prepareFromExistingPath(
                        hero.transform,
                        hero.movement
                    );
                    break;
                }
                else
                {
                    path.clear();
                    break;
                }
                //* NOTE: Possible fallthrough to ACT_TO_TARGET
            }

            //* NOTE: Possible fallthrough from ACT_TO_TARGET
            case InputId::ACT_IN_PLACE:
            {
                //* Interact
                if ( mapIO.objects.ids.contains( Convert::worldToTile( hero.position ) ) )
                {
                    size_t objectId{ mapIO.objects.ids.at( Convert::worldToTile( hero.position ) ) };

                    //* Wait if nothing to interact
                    if ( mapIO.objects.eventIds.contains( objectId ) )
                    {
#if defined( DEBUG )
                        snx::debug::cliLog( "Hero interacts.\n" );
#endif
                        EnergyModule::consume( hero.energy );

                        snx::EventDispatcher::notify( mapIO.objects.eventIds.at( objectId ) );

                        break;
                    }
                }

#if defined( DEBUG )
                snx::debug::cliLog( "Hero waits.\n" );
#endif
                snx::Logger::log( "Hero waits...\n" );

                EnergyModule::consume( hero.energy );

                hero.health = HealthModule::regenerate( hero.health );

                break;
            }
        }

        return hero;
    }
}
