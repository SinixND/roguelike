#include "Game.h"

#include "AISystem.h"
#include "Cursor.h"
#include "Enemies.h"
#include "EventId.h"
#include "GameCamera.h"
#include "Hero.h"
#include "InputActionId.h"
#include "InputHandler.h"
#include "InputMappings.h"
#include "Logger.h"
#include "MovementSystem.h"
#include "Objects.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "RenderId.h"
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

    inputMappings = defaultInputMappings();

    //* Setup events
    setupGameEvents();

#if defined(DEBUG)
    snx::PublisherStatic::publish(EventId::NEXT_LEVEL);
#endif
}

void Game::processInput(Cursor& cursor)
{
    //* Take input from mouse, keys or gestures
    inputAction = checkKeyboard(
        inputHandler,
        inputMappings);

    if (inputAction != InputActionId::NONE)
    {
        return;
    }

    inputAction = checkMouse(inputMappings, cursor.isActive);

    if (inputAction != InputActionId::NONE)
    {
        return;
    }

    inputAction = checkGesture(inputHandler);
}

void Game::updateState(
    GameCamera const& gameCamera,
    Cursor const& cursor)
{
    //* AI
    if (!isMultiFrameActionActive)
    {
        isMultiFrameActionActive = AISystem::takeActions(
            world,
            hero,
            activeEnemyId,
            gameCamera,
            turn);
    }

    //* Hero
    if (!isMultiFrameActionActive
        && hero.energy.isReady())
    {
        isMultiFrameActionActive = UserInputSystem::takeAction(
            inputAction,
            hero,
            cursor,
            *world.currentMap,
            gameCamera);
    }

    //* Update instant actions
    if (!isMultiFrameActionActive)
    {
        replaceDeadEnemies(
            world.currentMap->enemies,
            *world.currentMap);
    }
    //* Update multi-frame actions
    else
    {
        //* Update hero
        MovementSystem::update(
            hero.transform,
            hero.movement,
            hero.position,
            hero.energy,
            hero.position);

        //* Update enemies
        MovementSystem::updateEnemies(
            world.currentMap->enemies,
            hero.position);
    }

    //* Regenerate energy if no action in progress
    if (!isMultiFrameActionActive
        && !hero.energy.isReady()
        && !activeEnemyId)
    {
        bool isUnitReady{false};

        //* Regenerate until one unit becomes ready
        while (!isUnitReady)
        {
            isUnitReady = hero.energy.regenerate();
            isUnitReady |= regenerateEnergies(world.currentMap->enemies);
        }

        //* Progess turn when hero is ready
        if (hero.energy.isReady())
        {
            ++turn;
            snx::Logger::setStamp(std::to_string(turn));
        }
    }
}

void Game::setupGameEvents()
{
    snx::PublisherStatic::addSubscriber(
        EventId::MULTIFRAME_ACTION_ACTIVE,
        [&]()
        {
            isMultiFrameActionActive = true;
        });

    snx::PublisherStatic::addSubscriber(
        EventId::MULTIFRAME_ACTION_DONE,
        [&]()
        {
            isMultiFrameActionActive = false;
        });

    snx::PublisherStatic::addSubscriber(
        EventId::NEXT_LEVEL,
        [&]()
        {
            snx::Logger::log("Entered next level");

            world.increaseMapLevel();

            //* Place Hero on the map entry position
            hero.position.changeTo(Vector2I{0, 0});

            snx::PublisherStatic::publish(EventId::HERO_MOVED);
            snx::PublisherStatic::publish(EventId::HERO_POSITION_CHANGED);
            snx::PublisherStatic::publish(EventId::MAP_CHANGE);
        });

    snx::PublisherStatic::addSubscriber(
        EventId::PREVIOUS_LEVEL,
        [&]()
        {
            snx::Logger::log("Entered previous level");

            world.decreaseMapLevel();

            //* Place Hero on the map exit
            auto const& objects{world.currentMap->objects};
            auto const& renderIds{objects.renderIds.values()};
            auto const& positions{objects.positions.values()};

            for (size_t i{0}; i < renderIds.size(); ++i)
            {
                if (renderIds.at(i) == RenderId::DESCEND)
                {
                    hero.position.changeTo(
                        positions.at(i).tilePosition());
                }
            }

            snx::PublisherStatic::publish(EventId::HERO_MOVED);
            snx::PublisherStatic::publish(EventId::HERO_POSITION_CHANGED);
            snx::PublisherStatic::publish(EventId::MAP_CHANGE);
        });
}
