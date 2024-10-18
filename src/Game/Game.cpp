#include "Game.h"

#include "Cursor.h"
#include "Enemies.h"
#include "Event.h"
#include "GameCamera.h"
#include "Hero.h"
#include "Logger.h"
#include "Objects.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "RenderID.h"
#include "World.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#if defined(DEBUG)
#include "RNG.h"
#endif

void Game::init()
{
#if defined(DEBUG)
    snx::RNG::seed(1);
#endif

    hero.init();

    inputHandler_.setDefaultInputMappings();

    //* Setup events
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

            world.increaseMapLevel();

            hero.position.changeTo(Vector2I{0, 0});

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

            world.decreaseMapLevel();

            for (PositionComponent const& position : world.currentMap->objects.positions.values())
            {
                if (!(world.currentMap->objects.renderIDs.at(position.tilePosition()) == RenderID::descend))
                {
                    continue;
                }

                hero.position.changeTo(position.tilePosition());
            }

            --turn_;

            snx::PublisherStatic::publish(Event::heroMoved);
            snx::PublisherStatic::publish(Event::heroPositionChanged);
            snx::PublisherStatic::publish(Event::mapChange);
        });
}

void Game::processInput(Cursor& cursor)
{
    //* Allow input if hero is ready (= full energy)
    if (hero.energy.isReady())
    {
        //* Take input from mouse, keys or gestures
        //* Continuous movement done by repeating previous input if modifier is active
        inputHandler_.takeInput(cursor.isActive);
    }
}

void Game::updateState(
    GameCamera const& gameCamera,
    Cursor const& cursor)
{
    //* Cycle enemies once to check for action
    bool allEnemiesChecked{false};

    if (
        !actionsInProgress_
        && !hero.energy.isReady())
    {
        allEnemiesChecked = world.currentMap->enemies.checkForAction(
            *world.currentMap,
            hero.position.tilePosition(),
            gameCamera);
    }

    //* Regenerate energy if no action in progress
    if (
        !actionsInProgress_
        && !hero.energy.isReady()
        && allEnemiesChecked)
    {
        //* Regenerate until one unit becomes ready
        bool isUnitReady{false};

        while (!isUnitReady)
        {
            isUnitReady = hero.energy.regenerate();
            isUnitReady = world.currentMap->enemies.regenerate() || isUnitReady;
        }
    }

    //* Trigger potential hero action
    inputHandler_.triggerAction(
        hero,
        cursor,
        *world.currentMap,
        gameCamera);

    //* Update hero movment
    hero.movement.update(
        hero.position,
        hero.energy,
        *world.currentMap,
        hero.position);

    //* Update enemies
    world.currentMap->enemies.update(*world.currentMap, hero.position);
}
