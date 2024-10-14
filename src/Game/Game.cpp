#include "Game.h"

#include "Cursor.h"
#include "Enemies.h"
#include "Event.h"
#include "GameCamera.h"
#include "Hero.h"
#include "Logger.h"
#include "Objects.h"
#include "Position.h"
#include "PublisherStatic.h"
#include "RenderID.h"
#include "World.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <utility>

#if defined(DEBUG)
#include "RNG.h"
#endif

void Game::init()
{
#if defined(DEBUG)
    snx::RNG::seed(1);
#endif

    hero_.init();

    inputHandler_.setDefaultInputMappings();

    // Setup events
    setupGameEvents();

#if defined(DEBUG)
    snx::PublisherStatic::publish(Event::nextLevel);
#endif
}

void Game::setupGameEvents()
{
    snx::PublisherStatic::addSubscriber(
        Event::actionInProgress,
        [&]()
        {
            actionsInProgress_ = true;
        });

    snx::PublisherStatic::addSubscriber(
        Event::actionFinished,
        [&]()
        {
            actionsInProgress_ = false;
        });

    snx::PublisherStatic::addSubscriber(
        Event::nextLevel,
        [&]()
        {
            snx::Logger::log("Enter next Level...");

            world_.increaseMapLevel();

            hero_.position().changeTo(Vector2I{0, 0});

            ++turn_;

            snx::PublisherStatic::publish(Event::heroMoved);
            snx::PublisherStatic::publish(Event::heroPositionChanged);
            snx::PublisherStatic::publish(Event::mapChange);
        });

    snx::PublisherStatic::addSubscriber(
        Event::previousLevel,
        [&]()
        {
            snx::Logger::log("Enter previous Level...");

            world_.decreaseMapLevel();

            for (Position const& position : world_.currentMap().objects_.positions().values())
            {
                if (!(world_.currentMap().objects_.renderID(position.tilePosition()) == RenderID::descend))
                {
                    continue;
                }

                hero_.position().changeTo(position.tilePosition());
            }

            --turn_;

            snx::PublisherStatic::publish(Event::heroMoved);
            snx::PublisherStatic::publish(Event::heroPositionChanged);
            snx::PublisherStatic::publish(Event::mapChange);
        });
}

void Game::processInput()
{
    // Allow input if hero is ready (= full energy)
    if (hero_.energy().isReady())
    {
        // Take input from mouse, keys or gestures
        // Continuous movement done by repeating previous input if modifier is active
        inputHandler_.takeInput();
    }
}

void Game::updateState(
    GameCamera const& gameCamera,
    Cursor const& cursor)
{
    // Cycle enemies once to check for action
    bool allEnemiesChecked{false};

    if (
        !actionsInProgress_
        && !hero_.energy().isReady())
    {
        allEnemiesChecked = world_.currentMap().enemies_.checkForAction(
            world_.currentMap(),
            hero_.position().tilePosition(),
            gameCamera);
    }

    // Regenerate energy if no action in progress
    if (
        !actionsInProgress_
        && !hero_.energy().isReady()
        && allEnemiesChecked)
    {
        // Regenerate until one unit becomes ready
        bool isUnitReady{false};

        while (!isUnitReady)
        {
            isUnitReady = hero_.energy().regenerate();
            isUnitReady = world_.currentMap().enemies_.regenerate() || isUnitReady;
        }
    }

    // Trigger potential hero action
    inputHandler_.triggerAction(
        hero_,
        cursor,
        world_.currentMap(),
        gameCamera);

    // Update hero movment
    hero_.movement().update(
        hero_.position(),
        hero_.energy(),
        world_.currentMap(),
        hero_.position());

    // Update enemies
    world_.currentMap().enemies_.update(world_.currentMap(), hero_.position());
}

Hero const& Game::hero() const
{
    return hero_;
}

World const& Game::world() const
{
    return world_;
}

World& Game::world()
{
    return const_cast<World&>(std::as_const(*this).world());
}