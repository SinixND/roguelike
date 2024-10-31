#include "Game.h"

#include "AISystem.h"
#include "Cursor.h"
#include "EnemySoA.h"
#include "Event.h"
#include "GameCamera.h"
#include "Hero.h"
#include "Logger.h"
#include "MovementSystem.h"
#include "ObjectSoA.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "RenderID.h"
#include "UserInputSystem.h"
#include "World.h"
#include "raylibEx.h"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <string>

#if defined(DEBUG)
#include "RNG.h"
#endif

void Game::init()
{
#if defined(DEBUG)
    snx::RNG::seed(1);
#endif

    UserInputSystem::setDefaultInputMappings(userInput_);

    //* Setup events
    setupGameEvents();

#if defined(DEBUG)
    snx::PublisherStatic::publish(Event::NEXT_LEVEL);
#endif
}

void Game::setupGameEvents()
{
    snx::PublisherStatic::addSubscriber(
        Event::ACTION_IN_PROGRESS,
        [&]()
        {
            actionsInProgress_ = true;
        });

    snx::PublisherStatic::addSubscriber(
        Event::ACTION_FINISHED,
        [&]()
        {
            actionsInProgress_ = false;
        });

    snx::PublisherStatic::addSubscriber(
        Event::NEXT_LEVEL,
        [&]()
        {
            snx::Logger::log("Entered next level");

            world.increaseMapLevel();

            //* Place Hero on the map entry position
            hero.position.changeTo(Vector2I{0, 0});

            snx::PublisherStatic::publish(Event::HERO_MOVED);
            snx::PublisherStatic::publish(Event::HERO_POSITION_CHANGED);
            snx::PublisherStatic::publish(Event::MAP_CHANGE);
        });

    snx::PublisherStatic::addSubscriber(
        Event::PREVIOUS_LEVEL,
        [&]()
        {
            snx::Logger::log("Entered previous level");

            world.decreaseMapLevel();

            //* Place Hero on the map exit
            auto const& objects{world.currentMap->objects};
            auto const& renderIDs{objects.renderIDs.values()};
            auto const& positions{objects.positions.values()};

            for (size_t i{0}; i < renderIDs.size(); ++i)
            {
                if (renderIDs.at(i) == RenderID::DESCEND)
                {
                    hero.position.changeTo(
                        positions.at(i).tilePosition());
                }
            }

            snx::PublisherStatic::publish(Event::HERO_MOVED);
            snx::PublisherStatic::publish(Event::HERO_POSITION_CHANGED);
            snx::PublisherStatic::publish(Event::MAP_CHANGE);
        });
}

void Game::processInput(Cursor& cursor)
{
    //* Take input from mouse, keys or gestures
    //* Continuous movement done by repeating previous input if modifier is active
    UserInputSystem::takeInput(
        userInput_,
        cursor.isActive);
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
        Map& map{*world.currentMap};

        allEnemiesChecked = AISystem::checkForAction(
            map.enemies,
            map,
            hero.position.tilePosition(),
            hero.health,
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

        ++turn_;
        snx::Logger::setStamp(std::to_string(turn_));
    }

    //* Trigger potential hero action
    UserInputSystem::triggerAction(
        userInput_,
        hero,
        cursor,
        *world.currentMap,
        gameCamera);

    //* Update hero movement
    MovementSystem::update(
        hero.movement,
        hero.position,
        hero.energy,
        *world.currentMap,
        hero.position);

    //* Update enemies
    world.currentMap->enemies.update(*world.currentMap, hero.position);
}

int Game::turn() const
{
    return turn_;
}