#include "Game.h"

#include "AISystem.h"
#include "ActionSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Enemies.h"
#include "EnergyComponent.h"
#include "EventId.h"
#include "GameCamera.h"
#include "Hero.h"
#include "Levels.h"
#include "MovementSystem.h"
#include "Objects.h"
#include "PublisherStatic.h"
#include "RenderId.h"
#include "raylibEx.h"
#include <Logger.h>
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#if defined( DEBUG )
#include "Debugger.h"
#include "RNG.h"
#endif

[[nodiscard]]
Game const& setupGameEvents( Game& game )
{
    snx::PublisherStatic::addSubscriber(
        EventId::MULTIFRAME_ACTION_ACTIVE,
        [&]()
        {
            game.isMultiFrameActionActive = true;
        }
    );

    snx::PublisherStatic::addSubscriber(
        EventId::MULTIFRAME_ACTION_DONE,
        [&]()
        {
            game.isMultiFrameActionActive = false;
        }
    );

    snx::PublisherStatic::addSubscriber(
        EventId::NEXT_LEVEL,
        [&]()
        {
            snx::Logger::log( "Entered next level\n" );

            game.levels.increaseMapLevel();

            //* Place Hero on the map entry position
            game.hero.position = Convert::tileToWorld( Vector2I{ 0, 0 } );

            snx::PublisherStatic::publish( EventId::HERO_MOVED );
            snx::PublisherStatic::publish( EventId::HERO_POSITION_CHANGED );
            snx::PublisherStatic::publish( EventId::MAP_CHANGE );
        }
    );

    snx::PublisherStatic::addSubscriber(
        EventId::PREVIOUS_LEVEL,
        [&]()
        {
            snx::Logger::log( "Entered previous level\n" );

            game.levels.decreaseMapLevel();

            //* Place Hero on the map exit
            auto const& objects{ game.levels.currentMap->objects };
            auto const& renderIds{ objects.renderIds.values() };
            auto const& positions{ objects.positions.values() };

            for ( size_t idx{ 0 }; idx < renderIds.size(); ++idx )
            {
                if ( renderIds[idx] == RenderId::DESCEND )
                {
                    game.hero.position = positions[idx];
                }
            }

            snx::PublisherStatic::publish( EventId::HERO_MOVED );
            snx::PublisherStatic::publish( EventId::HERO_POSITION_CHANGED );
            snx::PublisherStatic::publish( EventId::MAP_CHANGE );
        }
    );

    snx::PublisherStatic::addSubscriber(
        EventId::INTERRUPT_MOVEMENT,
        [&]()
        {
            game.hero.movement.path.clear();
        }
    );

    return game;
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
        // hero.movement,
        dt
    );

    snx::PublisherStatic::publish( EventId::HERO_MOVED );

    if ( oldPosition != Convert::worldToTile( hero.position ) )
    {
        snx::PublisherStatic::publish( EventId::HERO_POSITION_CHANGED );
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
            // enemies.movements[idx],
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
        game.levels.currentMap->enemies = continueEnemyMovements(
            game.levels.currentMap->enemies,
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
    if ( game.hero.energy.state == EnergyComponent::State::READY )
    {
        game.hero = ActionSystem::executeAction(
            game.hero,
            *game.levels.currentMap,
            cursor,
            gameCamera,
            currentInputId
        );
    }

    //* Enemies
    else
    {
        game.levels.currentMap->enemies = AISystem::executeNextAction(
            game.levels.currentMap->enemies,
            game.activeEnemyId,
            game.hero,
            *game.levels.currentMap,
            gameCamera,
            game.turn
        );
    }

    return game;
}

namespace GameModule
{
    [[nodiscard]]
    Game const& init( Game& game )
    {
        game = setupGameEvents( game );

#if defined( DEBUG )
        snx::PublisherStatic::publish( EventId::NEXT_LEVEL );
#endif

        return game;
    }

    [[nodiscard]]
    Game const& update(
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
        game.levels.currentMap->enemies = EnemiesModule::replaceDead(
            game.levels.currentMap->enemies,
            game.levels.currentMap->tiles
        );

        //* Skip energy regeneration while a unit is ready
        if ( game.hero.energy.state == EnergyComponent::State::READY
             || game.activeEnemyId )
        {
            return game;
        }

        //* Regenerate energy until a unit becomes ready
        bool isUnitReady{ false };

        //* Regenerate until one unit becomes ready
#if defined( DEBUG )
        snx::debug::cliLog( "No action left. Regen units.\n" );
#endif
        while ( !isUnitReady )
        {
            //* Unit is ready when regenerate is _not_ successful
            isUnitReady = !EnergyModule::regenerate( game.hero.energy );
            isUnitReady |= !EnemiesModule::regenerate( game.levels.currentMap->enemies.energies );
        }

        //* Increment turn when hero is ready
        if ( game.hero.energy.state == EnergyComponent::State::READY )
        {
#if defined( DEBUG )
            snx::debug::cliLog( "Hero ready. Next Turn\n\n" );
#endif
            ++game.turn;
            snx::Logger::setTurn( game.turn );
        }

        return game;
    }
}
