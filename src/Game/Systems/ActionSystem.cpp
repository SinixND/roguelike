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
    Hero* heroIO,
    Map* mapIO,
    Vector2I direction
)
{
    bool isActionMultiFrame = false;

    Vector2I target{
        Vector2Add(
            Convert::worldToTile( heroIO->position ),
            direction
        )
    };

    if ( mapIO->enemies.ids.contains( target ) )
    {
        snx::Logger::log( "Hero deals " );

        DamageSystem::attack(
            heroIO->damage,
            mapIO->enemies.healths[mapIO->enemies.ids[target]]
        );

        EnergyModule::consume( &heroIO->energy );
    }
    else if ( !CollisionSystem::checkCollision(
                  mapIO->tiles,
                  mapIO->enemies,
                  target,
                  Convert::worldToTile( heroIO->position )
              ) )
    {
        MovementSystem::prepareByDirection(
            heroIO->movement,
            direction,
            heroIO->transform
        );

        EnergyModule::consume( &heroIO->energy );

        isActionMultiFrame = true;
    }

    return isActionMultiFrame;
}

namespace ActionSystem
{
    bool executeAction(
        Hero* heroIO,
        Map* mapIO,
        Cursor const& cursor,
        GameCamera const& gameCamera,
        InputId currentInput
    )
    {
        bool isActionMultiFrame{ false };

        switch ( currentInput )
        {
            default:
            case InputId::NONE:
            {
                if ( !heroIO->movement.path.empty()
                     && !heroIO->transform.speed )
                {
                    if ( !CollisionSystem::checkCollision(
                             mapIO->tiles,
                             mapIO->enemies,
                             heroIO->movement.path.rbegin()[1],
                             Convert::worldToTile( heroIO->position )
                         ) )
                    {
                        MovementSystem::prepareFromExistingPath(
                            heroIO->movement,
                            heroIO->transform
                        );

                        isActionMultiFrame = true;
                    }
                    else
                    {
                        MovementSystem::resetTransform( heroIO->transform );
                        heroIO->movement.path.clear();
                    }
                }
                break;
            }

            case InputId::ACT_UP:
            {
                isActionMultiFrame = processDirectionalInput(
                    heroIO,
                    mapIO,
                    Directions::up
                );

                break;
            }

            case InputId::ACT_LEFT:
            {
                isActionMultiFrame = processDirectionalInput(
                    heroIO,
                    mapIO,
                    Directions::left
                );

                break;
            }

            case InputId::ACT_DOWN:
            {
                isActionMultiFrame = processDirectionalInput(
                    heroIO,
                    mapIO,
                    Directions::down
                );

                break;
            }

            case InputId::ACT_RIGHT:
            {
                isActionMultiFrame = processDirectionalInput(
                    heroIO,
                    mapIO,
                    Directions::right
                );

                break;
            }

            case InputId::TARGET:
            {
                std::vector<Vector2I> path{ PathfinderSystem::findPath(
                    *mapIO,
                    Convert::worldToTile( heroIO->position ),
                    Convert::worldToTile( cursor.position ),
                    gameCamera
                ) };

                if ( !path.empty() )
                {
                    if ( mapIO->enemies.ids.contains( path.rbegin()[1] ) )
                    {
                        snx::Logger::log( "Hero deals " );

                        DamageSystem::attack(
                            heroIO->damage,
                            mapIO->enemies.healths[mapIO->enemies.ids[path.rbegin()[1]]]
                        );

                        EnergyModule::consume( &heroIO->energy );

                        path.clear();
                    }
                    else if ( !CollisionSystem::checkCollision(
                                  mapIO->tiles,
                                  mapIO->enemies,
                                  path.rbegin()[1],
                                  Convert::worldToTile( heroIO->position )
                              ) )
                    {
                        MovementSystem::prepareByNewPath(
                            heroIO->movement,
                            path
                        );

                        MovementSystem::prepareFromExistingPath(
                            heroIO->movement,
                            heroIO->transform
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
                if ( mapIO->objects.ids.contains( Convert::worldToTile( cursor.position ) ) )
                {
                    size_t objectId{ mapIO->objects.ids[Convert::worldToTile( heroIO->position )] };

                    //* Wait if nothing to interact
                    if ( mapIO->objects.events.contains( objectId ) )
                    {
                        snx::PublisherStatic::publish( mapIO->objects.events[objectId] );

                        break;
                    }
                }

                snx::Logger::log( "Hero waits..." );

                EnergyModule::consume( &heroIO->energy );

                heroIO->health = HealthModule::regenerate( heroIO->health );

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
