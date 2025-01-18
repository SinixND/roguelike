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
#include "MovementSystem.h"
#include "Objects.h"
#include "PublisherStatic.h"
#include "RenderId.h"
#include "World.h"
#include "raylibEx.h"
#include <Logger.h>
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#if defined( DEBUG )
#include "RNG.h"
#endif

void GameModule::init( Game& game )
{
#if defined( DEBUG )
    snx::RNG::seed( 1 );
#endif

    //* Setup events
    GameModule::setupGameEvents( game );

#if defined( DEBUG )
    snx::PublisherStatic::publish( EventId::NEXT_LEVEL );
#endif
}

void GameModule::update(
    Game& game,
    GameCamera const& gameCamera,
    Cursor const& cursor,
    InputId currentInputId,
    float dt
)
{
    //* AI
    if ( !game.isMultiFrameActionActive )
    {
        game.isMultiFrameActionActive = AISystem::takeActions(
            game.world,
            game.hero,
            game.activeEnemyId,
            gameCamera,
            game.turn
        );
    }

    //* Hero
    if ( !game.isMultiFrameActionActive
         && game.hero.energy.state == EnergyState::READY )
    {
        game.isMultiFrameActionActive = ActionSystem::takeAction(
            currentInputId,
            game.hero,
            cursor,
            *game.world.currentMap,
            gameCamera
        );
    }

    //* Update instant actions
    if ( !game.isMultiFrameActionActive )
    {
        EnemiesModule::replaceDead(
            game.world.currentMap->enemies,
            game.world.currentMap->tiles
        );
    }
    //* Update multi-frame actions
    else
    {
        //* Update hero
        MovementSystem::update(
            game.hero.transform,
            // game.hero.movement,
            game.hero.position,
            game.hero.energy,
            game.hero.position,
            dt
        );

        //* Update enemies
        MovementSystem::updateEnemies(
            game.world.currentMap->enemies,
            game.hero.position,
            dt
        );
    }

    //* Regenerate energy if no action in progress
    if ( !game.isMultiFrameActionActive
         && game.hero.energy.state == EnergyState::NOT_READY
         && !game.activeEnemyId )
    {
        bool isUnitReady{ false };

        //* Regenerate until one unit becomes ready
        while ( !isUnitReady )
        {
            isUnitReady = EnergyModule::regenerate( game.hero.energy );
            isUnitReady |= EnemiesModule::regenerate( game.world.currentMap->enemies.energies );
        }

        //* Increment turn when hero is ready
        if ( game.hero.energy.state == EnergyState::READY )
        {
            ++game.turn;
            snx::Logger::setStamp( std::to_string( game.turn ) );
        }
    }
}

void GameModule::setupGameEvents( Game& game )
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
            snx::Logger::log( "Entered next level" );

            WorldModule::increaseMapLevel( game.world );

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
            snx::Logger::log( "Entered previous level" );

            WorldModule::decreaseMapLevel( game.world );

            //* Place Hero on the map exit
            auto const& objects{ game.world.currentMap->objects };
            auto const& renderIds{ objects.renderIds.values() };
            auto const& positions{ objects.positions.values() };

            for ( size_t idx{ 0 }; idx < renderIds.size(); ++idx )
            {
                if ( renderIds.at( idx ) == RenderId::DESCEND )
                {
                    game.hero.position = positions.at( idx );
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
            MovementSystem::resetTransform( game.hero.transform );
            game.hero.movement.path.clear();
        }
    );
}
