#include "ActionSystem.h"

// #define DEBUG_HERO_ACTIONS

#include "AttackComponent.h"
#include "CollisionSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Directions.h"
#include "EmptyComponent.h"
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
Hero handleInputToTarget(
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

        return hero;
    }

    //* Move
    else if ( map.tiles.isSolids.contains( map.tiles.ids.at( target ) ) )
    {
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
        snx::Debugger::cliLog( "Add move component to hero\n" );
#endif

        hero.move = std::make_shared<MoveComponent>( target );
    }

    return hero;
}

[[nodiscard]]
Hero handleInputInPlace(
    Hero& hero,
    Map const& map
)
{
    //* Interact (if object exists and has a event)
    if ( map.objects.ids.contains( Convert::worldToTile( hero.position ) )
         && map.objects.eventIds.contains( map.objects.ids.at( Convert::worldToTile( hero.position ) ) ) )
    {
        hero.interact = std::make_shared<EmptyComponent>();

        return hero;
        ;
    }

#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
    snx::Debugger::cliLog( "Hero waits.\n" );
#endif
    snx::Logger::log( "Hero waits...\n" );

    hero.wait = std::make_shared<EmptyComponent>();

    return hero;
    ;
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
                heroIO = handleInputToTarget(
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
                heroIO = handleInputToTarget(
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
                heroIO = handleInputToTarget(
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
                heroIO = handleInputToTarget(
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
                    heroIO = handleInputToTarget(
                        heroIO,
                        map,
                        path.rbegin()[1]
                    );

                    path.pop_back();

                    *heroIO.path = path;
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
