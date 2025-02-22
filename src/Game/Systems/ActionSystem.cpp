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

Hero const& processDirectionalInput(
    Hero& heroIO,
    Map& mapIO,
    Vector2I direction
)
{
    Vector2I target{
        Vector2Add(
            Convert::worldToTile( heroIO.position ),
            direction
        )
    };

    //* Attack
    if ( mapIO.enemies.ids.contains( target ) )
    {
#if defined( DEBUG )
        snx::debug::cliLog( "Hero attacks.\n" );
#endif
        snx::Logger::log( "Hero deals " );
        EnergyModule::consume( heroIO.energy );

        HealthModule::damage(
            mapIO.enemies.healths[mapIO.enemies.ids.index( target )],
            DamageModule::damageRNG( heroIO.damage )
        );
    }
    //* Move
    else if ( !CollisionSystem::checkCollision(
                  mapIO.tiles,
                  mapIO.enemies,
                  target,
                  Convert::worldToTile( heroIO.position )
              ) )
    {
#if defined( DEBUG )
        snx::debug::cliLog( "Hero moves.\n" );
#endif
        EnergyModule::consume( heroIO.energy );

        heroIO.transform = MovementSystem::prepareByDirection(
            heroIO.transform,
            heroIO.movement,
            direction
        );
    }

    return heroIO;
}

namespace ActionSystem
{
    Hero const& executeAction(
        Hero& heroIO,
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
                if ( !heroIO.movement.path.empty()
                     && !heroIO.transform.speed )
                {
                    //* Move
                    if ( !CollisionSystem::checkCollision(
                             mapIO.tiles,
                             mapIO.enemies,
                             heroIO.movement.path.rbegin()[1],
                             Convert::worldToTile( heroIO.position )
                         ) )
                    {
#if defined( DEBUG )
                        snx::debug::cliLog( "Hero moves.\n" );
#endif
                        EnergyModule::consume( heroIO.energy );

                        heroIO.transform = MovementSystem::prepareFromExistingPath(
                            heroIO.transform,
                            heroIO.movement
                        );
                    }
                    //* Stop
                    else
                    {
                        MovementSystem::resetTransform( heroIO.transform );
                        heroIO.movement.path.clear();
                    }
                }
                break;
            }

            case InputId::ACT_UP:
            {
                heroIO = processDirectionalInput(
                    heroIO,
                    mapIO,
                    Directions::up
                );

                break;
            }

            case InputId::ACT_LEFT:
            {
                heroIO = processDirectionalInput(
                    heroIO,
                    mapIO,
                    Directions::left
                );

                break;
            }

            case InputId::ACT_DOWN:
            {
                heroIO = processDirectionalInput(
                    heroIO,
                    mapIO,
                    Directions::down
                );

                break;
            }

            case InputId::ACT_RIGHT:
            {
                heroIO = processDirectionalInput(
                    heroIO,
                    mapIO,
                    Directions::right
                );

                break;
            }

            //* NOTE: Fall through possible to ACT_IN_PLACE if cursor.position == hero.position
            case InputId::ACT_TO_TARGET:
            {
                std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
                    Convert::worldToTile( heroIO.position ),
                    Convert::worldToTile( cursor.position ),
                    mapIO,
                    gameCamera
                ) };

                if (
                    Convert::worldToTile( heroIO.position )
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
                    EnergyModule::consume( heroIO.energy );

                    snx::Logger::log( "Hero deals " );
                    HealthModule::damage(
                        mapIO.enemies.healths[mapIO.enemies.ids.index( path.rbegin()[1] )],
                        DamageModule::damageRNG( heroIO.damage )
                    );

                    path.clear();
                    break;
                }
                else if ( !CollisionSystem::checkCollision(
                              mapIO.tiles,
                              mapIO.enemies,
                              path.rbegin()[1],
                              Convert::worldToTile( heroIO.position )
                          ) )
                {
#if defined( DEBUG )
                    snx::debug::cliLog( "Hero moves.\n" );
#endif
                    EnergyModule::consume( heroIO.energy );

                    heroIO.movement = MovementSystem::prepareByNewPath(
                        heroIO.movement,
                        path
                    );

                    heroIO.transform = MovementSystem::prepareFromExistingPath(
                        heroIO.transform,
                        heroIO.movement
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
                if ( mapIO.objects.ids.contains( Convert::worldToTile( heroIO.position ) ) )
                {
                    size_t objectId{ mapIO.objects.ids.at( Convert::worldToTile( heroIO.position ) ) };

                    //* Wait if nothing to interact
                    if ( mapIO.objects.eventIds.contains( objectId ) )
                    {
#if defined( DEBUG )
                        snx::debug::cliLog( "Hero interacts.\n" );
#endif
                        EnergyModule::consume( heroIO.energy );

                        snx::EventDispatcher::notify( mapIO.objects.eventIds.at( objectId ) );

                        break;
                    }
                }

#if defined( DEBUG )
                snx::debug::cliLog( "Hero waits.\n" );
#endif
                snx::Logger::log( "Hero waits...\n" );

                EnergyModule::consume( heroIO.energy );

                heroIO.health = HealthModule::regenerate( heroIO.health );

                break;
            }
        }

        return heroIO;
    }
}
