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
    if ( map.objects.ids.contains( Convert::worldToTile( hero.position ) )
         && map.objects.eventIds.contains( map.objects.ids.at( Convert::worldToTile( hero.position ) ) ) )
    {
        hero.action = std::make_shared<ActionId>( ActionId::INTERACT );

        return hero;
    }

#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
    snx::Debugger::cliLog( "Hero waits.\n" );
#endif
    snx::Logger::log( "Hero waits...\n" );

    hero.action = std::make_shared<ActionId>( ActionId::WAIT );

    return hero;
}

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
        hero.attack = std::make_shared<AttackComponent>( target );
    }

    return hero;
}
[[nodiscard]]
Hero const& handleInputToDistantTarget(
    Hero& hero,
    Map const& map,
    Vector2I const& target,
    std::vector<Vector2I> const& path
)
{
    //* Move
    if ( map.tiles.isSolids.contains( map.tiles.ids.at( target ) ) )
    {
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
        snx::Debugger::cliLog( "Add move component to hero\n" );
#endif
        hero.move = std::make_shared<MoveComponent>(
            Vector2Subtract(
                target,
                Convert::worldToTile( hero.position )
            )
        );

        hero.path = path;
        hero.path.pop_back();
    }

    return hero;
}

namespace ActionSystem
{
    void update(
        Hero& heroIO,
        Map const& map,
        InputId currentInput,
        Cursor const& cursor,
        GameCamera const& gameCamera
    )
    {
        if ( !heroIO.isReady )
        {
            return;
        }

        switch ( currentInput )
        {
            default:
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

                if ( path.size() == 1 )
                {
                    heroIO = handleInputInPlace(
                        heroIO,
                        map
                    );
                }

                else if ( path.size() > 1 )
                {
                    heroIO = handleInputToDistantTarget(
                        heroIO,
                        map,
                        path.rbegin()[1],
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
