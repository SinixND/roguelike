#include "ActionSystem.h"

#include "CollisionSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Directions.h"
#include "GameCamera.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "InputId.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

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
        snx::Logger::log( "Hero deals " );

        HealthModule::damage(
            mapIO.enemies.healths[mapIO.enemies.ids[target]],
            DamageModule::damageRNG( heroIO.damage )
        );

        EnergyModule::consume( heroIO.energy );
    }
    //* Move
    else if ( !CollisionSystem::checkCollision(
                  mapIO.tiles,
                  mapIO.enemies,
                  target,
                  Convert::worldToTile( heroIO.position )
              ) )
    {
        heroIO.transform = MovementSystem::prepareByDirection(
            heroIO.transform,
            heroIO.movement,
            direction
        );

        EnergyModule::consume( heroIO.energy );
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

            case InputId::TARGET:
            {
                std::vector<Vector2I> path{ PathfinderSystem::findPath(
                    mapIO,
                    Convert::worldToTile( heroIO.position ),
                    Convert::worldToTile( cursor.position ),
                    gameCamera
                ) };

                if ( !path.empty() )
                {
                    if ( mapIO.enemies.ids.contains( path.rbegin()[1] ) )
                    {
                        snx::Logger::log( "Hero deals " );

                        HealthModule::damage(
                            mapIO.enemies.healths[mapIO.enemies.ids[path.rbegin()[1]]],
                            DamageModule::damageRNG( heroIO.damage )
                        );

                        EnergyModule::consume( heroIO.energy );

                        path.clear();
                    }
                    else if ( !CollisionSystem::checkCollision(
                                  mapIO.tiles,
                                  mapIO.enemies,
                                  path.rbegin()[1],
                                  Convert::worldToTile( heroIO.position )
                              ) )
                    {
                        heroIO.movement = MovementSystem::prepareByNewPath(
                            heroIO.movement,
                            path
                        );

                        heroIO.transform = MovementSystem::prepareFromExistingPath(
                            heroIO.transform,
                            heroIO.movement
                        );
                    }
                    else
                    {
                        path.clear();
                    }
                }

                break;
            }

            case InputId::ACT_IN_PLACE:
            {
                if ( mapIO.objects.ids.contains( Convert::worldToTile( cursor.position ) ) )
                {
                    size_t objectId{ mapIO.objects.ids[Convert::worldToTile( heroIO.position )] };

                    //* Wait if nothing to interact
                    if ( mapIO.objects.events.contains( objectId ) )
                    {
                        snx::PublisherStatic::publish( mapIO.objects.events[objectId] );

                        break;
                    }
                }

                snx::Logger::log( "Hero waits..." );

                EnergyModule::consume( heroIO.energy );

                heroIO.health = HealthModule::regenerate( heroIO.health );

                break;
            }
        }

        return heroIO;
    }
}
