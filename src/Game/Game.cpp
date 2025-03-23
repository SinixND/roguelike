#include "Game.h"

// #define DEBUG_GAME_LOOP

#include "AISystem.h"
#include "AttributeSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Enemies.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "GameState.h"
#include "GameSystem.h"
#include "Hero.h"
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
}
