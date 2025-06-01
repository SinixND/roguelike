#include "ActionSystem.h"

// #define DEBUG_HERO_ACTIONS

#include "ActionId.h"
#include "AttackComponent.h"
#include "CollisionSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Directions.h"
#include "GameCamera.h"
#include "Hero.h"
#include "InputId.h"
#include "Logger.h"
#include "Map.h"
#include "MoveComponent.h"
#include "PathfinderSystem.h"
#include "TileData.h"
#include <memory>

#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
#include "Debugger.h"
#endif

[[nodiscard]]
Hero const& handleInputInPlace(
    Hero& hero,
    Map const& map
)
{
    //* Interact (if object exists and has a event)
    if (
        map.objects.ids.contains(
            Convert::worldToTile(
                hero.position
            )
        )
        && map.objects.eventIds.contains(
            map.objects.ids.at(
                Convert::worldToTile(
                    hero.position
                )
            )
        )
    )
    {
        hero.energy = EnergyModule::exhaust( hero.energy );

        hero.action = std::make_shared<ActionId>( ActionId::INTERACT );

        hero.isIdle = false;

        return hero;
    }

#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
    snx::Debugger::cliLog( "Hero waits.\n" );
#endif
    snx::Logger::log( "Hero waits...\n" );

    hero.energy = EnergyModule::exhaust( hero.energy );

    hero.action = std::make_shared<ActionId>( ActionId::WAIT );

    hero.isIdle = false;

    return hero;
}

[[nodiscard]]
Hero const& handleInputToDistantTarget(
    Hero& hero,
    std::vector<Vector2I> const& path
)
{
    //* Path
    hero.path = path;
    //* TODO: CHANGE/REMOVE
    // hero.path.pop_back();

    return hero;
}

namespace ActionSystem
{
    [[nodiscard]]
    Hero const& handleInputToAdjacentTarget(
        Hero& hero,
        Map const& map,
        Vector2I const& target
    )
    {
        //* Attack
        if ( map.enemies.ids.contains( target ) )
        {
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
            snx::Debugger::cliLog( "Add attack component to hero.\n" );
#endif
            hero.energy = EnergyModule::exhaust( hero.energy );

            hero.attack = std::make_shared<AttackComponent>( target );

            hero.isIdle = false;
        }

        //* Move
        else if ( !map.tiles.isSolids.contains( map.tiles.ids.at( target ) ) )
        {
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
            snx::Debugger::cliLog( "Add move component to hero\n" );
#endif
            hero.energy = EnergyModule::exhaust( hero.energy );

            hero.move = std::make_shared<MoveComponent>(
                Vector2Subtract(
                    target,
                    Convert::worldToTile( hero.position )
                ),
                HeroData::SPEED_BASE,
                TileData::TILE_SIZE
            );

            assert( ( !hero.move->direction.x || !hero.move->direction.y ) && "Direction not orthogonal!" );

            hero.isIdle = false;

            //* TODO: CHANGE/REMOVE
            // snx::EventDispatcher::notify( EventId::MULTIFRAME_ACTIONS_ACTIVE );
        }

        return hero;
    }

    void update(
        Hero& heroIO,
        Map const& map,
        InputId currentInput,
        Cursor const& cursor,
        GameCamera const& gameCamera
    )
    {
        //* Interruptable path movement
        if ( currentInput != InputId::NONE )
        {
            heroIO.path.clear();
        }

        switch ( currentInput )
        {
            default:
            case InputId::NONE:
            {
                size_t pathSize{ heroIO.path.size() };

                if ( !pathSize )
                {
                    break;
                }

                else if ( pathSize == 1 )
                {
                    heroIO = handleInputInPlace(
                        heroIO,
                        map
                    );
                }

                else // if ( pathSize > 1 )
                {
                    heroIO = handleInputToAdjacentTarget(
                        heroIO,
                        map,
                        heroIO.path.rbegin()[1]
                    );
                    //* TODO: CHANGE/REMOVE
                    // }

                    // if ( pathSize > 2 )
                    // {
                    heroIO = handleInputToDistantTarget(
                        heroIO,
                        heroIO.path
                    );
                }
                break;
            }

            case InputId::ACT_UP:
            {
                heroIO = handleInputToAdjacentTarget(
                    heroIO,
                    map,
                    Vector2Add(
                        Convert::worldToTile( heroIO.position ),
                        Directions::up
                    )
                );

                break;
            }

            case InputId::ACT_LEFT:
            {
                heroIO = handleInputToAdjacentTarget(
                    heroIO,
                    map,
                    Vector2Add(
                        Convert::worldToTile( heroIO.position ),
                        Directions::left
                    )
                );

                break;
            }

            case InputId::ACT_DOWN:
            {
                heroIO = handleInputToAdjacentTarget(
                    heroIO,
                    map,
                    Vector2Add(
                        Convert::worldToTile( heroIO.position ),
                        Directions::down
                    )
                );

                break;
            }

            case InputId::ACT_RIGHT:
            {
                heroIO = handleInputToAdjacentTarget(
                    heroIO,
                    map,
                    Vector2Add(
                        Convert::worldToTile( heroIO.position ),
                        Directions::right
                    )
                );

                break;
            }

            case InputId::ACT_TO_TARGET:
            {
                std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
                    Convert::worldToTile( heroIO.position ),
                    Convert::worldToTile( cursor.position ),
                    map,
                    gameCamera
                ) };

                size_t pathSize{ path.size() };

                if ( !pathSize )
                {
                    break;
                }

                else if ( pathSize == 1 )
                {
                    heroIO = handleInputInPlace(
                        heroIO,
                        map
                    );
                }

                else // if ( pathSize > 1 )
                {
                    heroIO = handleInputToAdjacentTarget(
                        heroIO,
                        map,
                        path.rbegin()[1]
                    );
                }

                if ( pathSize > 2 )
                {
                    heroIO = handleInputToDistantTarget(
                        heroIO,
                        path
                    );
                }

                break;
            }

            case InputId::ACT_IN_PLACE:
            {
                heroIO = handleInputInPlace(
                    heroIO,
                    map
                );

                break;
            }
        }
    }
}
