#include "Game.h"

// #define DEBUG_GAME_LOOP

#include "AISystem.h"
#include "AttributeSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Enemies.h"
#include "EnergyComponent.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "GameCamera.h"
#include "GameState.h"
#include "Hero.h"
#include "MovementSystem.h"
#include "Objects.h"
#include "RenderId.h"
#include "World.h"
#include "raylibEx.h"
#include <Logger.h>
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#if defined( DEBUG )
#if defined( DEBUG_GAME_LOOP )
#include "Debugger.h"
#endif
#include "RNG.h"
#endif

void setupGameEvents( Game& game )
{
    snx::EventDispatcher::addListener(
        EventId::MULTIFRAME_ACTION_ACTIVE,
        [&]()
        {
            game.isMultiFrameActionActive = true;
        }
    );

    snx::EventDispatcher::addListener(
        EventId::MULTIFRAME_ACTION_DONE,
        [&]()
        {
            game.isMultiFrameActionActive = false;
        }
    );

    snx::EventDispatcher::addListener(
        EventId::NEXT_LEVEL,
        [&]()
        {
            snx::Logger::log( "Entered next level\n" );

            game.world.increaseMapLevel();

            //* Place Hero on the map entry position
            game.hero.position = Convert::tileToWorld( Vector2I{ 0, 0 } );

            snx::EventDispatcher::notify( EventId::HERO_MOVED );
            snx::EventDispatcher::notify( EventId::HERO_POSITION_CHANGED );
            snx::EventDispatcher::notify( EventId::MAP_CHANGE );
        }
    );

    snx::EventDispatcher::addListener(
        EventId::PREVIOUS_LEVEL,
        [&]()
        {
            snx::Logger::log( "Entered previous level\n" );

            game.world.decreaseMapLevel();

            //* Place Hero on the map exit
            auto const& objects{ game.world.currentMap->objects };
            auto const& renderIds{ objects.renderIds.values() };
            auto const& positions{ objects.positions.values() };

            for ( size_t idx{ 0 }; idx < renderIds.size(); ++idx )
            {
                if ( renderIds[idx] == RenderId::DESCEND )
                {
                    game.hero.position = positions[idx];
                }
            }

            snx::EventDispatcher::notify( EventId::HERO_MOVED );
            snx::EventDispatcher::notify( EventId::HERO_POSITION_CHANGED );
            snx::EventDispatcher::notify( EventId::MAP_CHANGE );
        }
    );

    snx::EventDispatcher::addListener(
        EventId::INTERRUPT_MOVEMENT,
        [&]()
        {
            game.hero.movement.path.clear();
        }
    );

    snx::EventDispatcher::addListener(
        EventId::NEXT_TURN,
        [&]()
        {
            ++game.turn;
        }
    );

    snx::EventDispatcher::addListener(
        EventId::LEVEL_UP,
        [&]()
        {
            game.state = GameState::LEVEL_UP;
        }
    );

    snx::EventDispatcher::addListener(
        EventId::LEVELED_UP,
        [&]()
        {
            game.state = GameState::DEFAULT;
            //* TODO: Update stats
        }
    );
}

[[nodiscard]]
Hero const& continueHeroMovement(
    Hero& hero,
    float dt
)
{
    Vector2I oldPosition{ Convert::worldToTile( hero.position ) };

    hero.position = MovementSystem::updateSingle(
        hero.position,
        hero.transform,
        dt
    );

    snx::EventDispatcher::notify( EventId::HERO_MOVED );

    if ( oldPosition != Convert::worldToTile( hero.position ) )
    {
        snx::EventDispatcher::notify( EventId::HERO_POSITION_CHANGED );
    }

    return hero;
}

[[nodiscard]]
Enemies const& continueEnemyMovements(
    Enemies& enemies,
    float dt
)
{
    Vector2I oldPosition{};

    for ( size_t idx{ 0 }; idx < enemies.transforms.size(); ++idx )
    {
        oldPosition = Convert::worldToTile( enemies.positions[idx] );

        //* Update movement
        enemies.positions[idx] = MovementSystem::updateSingle(
            enemies.positions[idx],
            enemies.transforms[idx],
            dt
        );

        //* Update ids_ key if tilePosition changed
        if ( oldPosition != Convert::worldToTile( enemies.positions[idx] ) )
        {
            enemies.ids.changeKey(
                oldPosition,
                Convert::worldToTile( enemies.positions[idx] )
            );
        }
    }

    return enemies;
}

[[nodiscard]]
Game const& continueMultiFrameActions(
    Game& game,
    float dt
)
{
    //* Hero
    if ( game.hero.transform.remainingDistance )
    {
        game.hero = continueHeroMovement(
            game.hero,
            dt
        );
    }
    //* Enemies
    else
    {
        game.world.currentMap->enemies = continueEnemyMovements(
            game.world.currentMap->enemies,
            dt
        );
    }

    return game;
}

[[nodiscard]]
Game const& executeInstantActions(
    Game& game,
    GameCamera const& gameCamera,
    Cursor const& cursor,
    InputId currentInputId
)
{
    //* Hero
    if ( game.hero.energy.isReady )
    {
        game.hero = HeroModule::executeAction(
            game.hero,
            *game.world.currentMap,
            cursor,
            gameCamera,
            currentInputId
        );
    }

    //* Enemies
    else
    {
        game.world.currentMap->enemies = AISystem::executeNextAction(
            game.world.currentMap->enemies,
            game.activeEnemyId,
            game.hero,
            *game.world.currentMap,
            gameCamera
        );
    }

    return game;
}

[[nodiscard]]
Game const& updateDefault(
    Game& game,
    GameCamera const& gameCamera,
    Cursor const& cursor,
    InputId currentInputId,
    float dt
)
{
    if ( !game.isMultiFrameActionActive )
    {
        game = executeInstantActions(
            game,
            gameCamera,
            cursor,
            currentInputId
        );
    }

    if ( game.isMultiFrameActionActive )
    {
        game = continueMultiFrameActions(
            game,
            dt
        );

        return game;
    }

    //* End turn
    if ( game.hero.health.current <= 0 )
    {
        snx::EventDispatcher::notify( EventId::GAME_OVER );
    }

    game.world.currentMap->enemies = EnemiesModule::replaceDead(
        game.world.currentMap->enemies,
        game.world.currentMap->tiles,
        game.world.currentMapLevel
    );

    //* Skip energy regeneration while a unit is ready
    if ( game.hero.energy.isReady
         || game.activeEnemyId )
    {
        return game;
    }

    //* Regenerate energy until a unit becomes ready
    bool isUnitReady{ false };

    //* Regenerate until one unit becomes ready
#if defined( DEBUG ) && defined( DEBUG_GAME_LOOP )
    snx::Debugger::cliLog( "No action left. Regen units.\n" );
#endif
    while ( !isUnitReady )
    {
        //* Unit is ready when regenerate is _not_ successful
        isUnitReady = EnergyModule::regenerate( game.hero.energy );
        isUnitReady |= EnemiesModule::regenerate( game.world.currentMap->enemies.energies );
    }

    //* Increment turn when hero is ready
    if ( game.hero.energy.isReady )
    {
#if defined( DEBUG ) && defined( DEBUG_GAME_LOOP )
        snx::Debugger::cliLog( "Hero ready. Next Turn\n\n" );
#endif
        snx::EventDispatcher::notify( EventId::NEXT_TURN );

        snx::Logger::incrementTurn();
    }

    return game;
}

namespace GameModule
{
    Game const& init( Game& game )
    {
#if defined( DEBUG )
        snx::RNG::seed( 1 );
#endif
        game.hero = Hero{};

        AttributeSystem::updateStats(
            game.hero.health,
            game.hero.energy,
            game.hero.attributes
        );

        game.world = World{};

        game.turn = 1;

        setupGameEvents( game );

#if defined( DEBUG )
        snx::EventDispatcher::notify( EventId::NEXT_LEVEL );
#endif

        return game;
    }

    Game const& update(
        Game& game,
        GameCamera const& gameCamera,
        Cursor const& cursor,
        InputId currentInputId,
        float dt
    )
    {
        switch ( game.state )
        {
            default:
            case GameState::DEFAULT:
            {
                game = updateDefault(
                    game,
                    gameCamera,
                    cursor,
                    currentInputId,
                    dt
                );
                break;
            }

            case GameState::LEVEL_UP:
            {
                //* ScreenGameOver handles input because it can be a UI selection
                break;
            }
        }

        return game;
    }
}
