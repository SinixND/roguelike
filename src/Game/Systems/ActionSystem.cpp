#include "ActionSystem.h"

//* #define DEBUG_GESTURES
//* #define DEBUG_GESTURE_EVENTS

#include "CollisionSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "DamageSystem.h"
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

bool processDirectionalInput(
    Hero& hero,
    Map& map,
    Vector2I direction
)
{
    bool isActionMultiFrame = false;

    Vector2I target{
        Vector2Add(
            Convert::worldToTile( hero.position ),
            direction
        )
    };

    if ( map.enemies.ids.contains( target ) )
    {
        snx::Logger::log( "Hero deals " );

        DamageSystem::attack(
            hero.damage,
            map.enemies.healths[map.enemies.ids[target]]
        );

        EnergyModule::consume( &hero.energy );
    }
    else if ( !CollisionSystem::checkCollision(
                  map.tiles,
                  map.enemies,
                  target,
                  Convert::worldToTile( hero.position )
              ) )
    {
        MovementSystem::prepareByDirection(
            hero.movement,
            direction,
            hero.transform
        );

        EnergyModule::consume( &hero.energy );

        isActionMultiFrame = true;
    }

    return isActionMultiFrame;
}

namespace ActionSystem
{
    bool takeAction(
        InputId currentInput,
        Hero& hero,
        Cursor const& cursor,
        Map& map,
        GameCamera const& gameCamera
    )
    {
        bool isActionMultiFrame{ false };

        switch ( currentInput )
        {
            default:
            case InputId::NONE:
            {
                if ( !hero.movement.path.empty()
                     && !hero.transform.speed )
                {
                    if ( !CollisionSystem::checkCollision(
                             map.tiles,
                             map.enemies,
                             hero.movement.path.rbegin()[1],
                             Convert::worldToTile( hero.position )
                         ) )
                    {
                        MovementSystem::prepareFromExistingPath(
                            hero.movement,
                            hero.transform
                        );

                        isActionMultiFrame = true;
                    }
                    else
                    {
                        MovementSystem::resetTransform( hero.transform );
                        hero.movement.path.clear();
                    }
                }
                break;
            }

            case InputId::ACT_UP:
            {
                isActionMultiFrame = processDirectionalInput(
                    hero,
                    map,
                    Directions::up
                );

                break;
            }

            case InputId::ACT_LEFT:
            {
                isActionMultiFrame = processDirectionalInput(
                    hero,
                    map,
                    Directions::left
                );

                break;
            }

            case InputId::ACT_DOWN:
            {
                isActionMultiFrame = processDirectionalInput(
                    hero,
                    map,
                    Directions::down
                );

                break;
            }

            case InputId::ACT_RIGHT:
            {
                isActionMultiFrame = processDirectionalInput(
                    hero,
                    map,
                    Directions::right
                );

                break;
            }

            case InputId::TARGET:
            {
                std::vector<Vector2I> path{ PathfinderSystem::findPath(
                    map,
                    Convert::worldToTile( hero.position ),
                    Convert::worldToTile( cursor.position ),
                    gameCamera
                ) };

                if ( !path.empty() )
                {
                    if ( map.enemies.ids.contains( path.rbegin()[1] ) )
                    {
                        snx::Logger::log( "Hero deals " );

                        DamageSystem::attack(
                            hero.damage,
                            map.enemies.healths[map.enemies.ids[path.rbegin()[1]]]
                        );

                        EnergyModule::consume( &hero.energy );

                        path.clear();
                    }
                    else if ( !CollisionSystem::checkCollision(
                                  map.tiles,
                                  map.enemies,
                                  path.rbegin()[1],
                                  Convert::worldToTile( hero.position )
                              ) )
                    {
                        MovementSystem::prepareByNewPath(
                            hero.movement,
                            path
                        );

                        MovementSystem::prepareFromExistingPath(
                            hero.movement,
                            hero.transform
                        );
                    }
                    else
                    {
                        path.clear();
                    }
                }

                isActionMultiFrame = !path.empty();

                break;
            }

            case InputId::ACT_IN_PLACE:
            {
                if ( map.objects.ids.contains( Convert::worldToTile( cursor.position ) ) )
                {
                    size_t objectId{ map.objects.ids[Convert::worldToTile( hero.position )] };

                    //* Wait if nothing to interact
                    if ( map.objects.events.contains( objectId ) )
                    {
                        snx::PublisherStatic::publish( map.objects.events[objectId] );

                        break;
                    }
                }

                snx::Logger::log( "Hero waits..." );

                EnergyModule::consume( &hero.energy );

                HealthModule::regenerate( hero.health );

                break;
            }

            case InputId::TOGGLE_CURSOR:
            {
                snx::PublisherStatic::publish( EventId::CURSOR_TOGGLE );

                break;
            }
        }

        return isActionMultiFrame;
    }
}
