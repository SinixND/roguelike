#include "UserInputSystem.h"
//* #define DEBUG_GESTURES
//* #define DEBUG_GESTURE_EVENTS

#include "Cursor.h"
#include "Debugger.h"
#include "Directions.h"
#include "Event.h"
#include "GameCamera.h"
#include "Hero.h"
#include "InputActionID.h"
#include "Logger.h"
#include "Map.h"
#include "PathfinderSystem.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "UserInputComponent.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void UserInputSystem::setDefaultInputMappings(
    UserInputComponent& userInputComponent)
{
    userInputComponent.bindKey(KEY_NULL, InputActionID::NONE);

    userInputComponent.bindKey(KEY_W, InputActionID::ACT_UP);
    userInputComponent.bindKey(KEY_K, InputActionID::ACT_UP);
    userInputComponent.bindKey(KEY_UP, InputActionID::ACT_UP);

    userInputComponent.bindKey(KEY_A, InputActionID::ACT_LEFT);
    userInputComponent.bindKey(KEY_K, InputActionID::ACT_LEFT);
    userInputComponent.bindKey(KEY_LEFT, InputActionID::ACT_LEFT);

    userInputComponent.bindKey(KEY_S, InputActionID::ACT_DOWN);
    userInputComponent.bindKey(KEY_J, InputActionID::ACT_DOWN);
    userInputComponent.bindKey(KEY_DOWN, InputActionID::ACT_DOWN);

    userInputComponent.bindKey(KEY_D, InputActionID::ACT_RIGHT);
    userInputComponent.bindKey(KEY_L, InputActionID::ACT_RIGHT);
    userInputComponent.bindKey(KEY_RIGHT, InputActionID::ACT_RIGHT);

    userInputComponent.bindKey(KEY_SPACE, InputActionID::ACT_IN_PLACE);

    userInputComponent.bindModifierKey(KEY_LEFT_SHIFT, InputActionID::MOD);

    userInputComponent.bindMouseButton(MOUSE_BUTTON_RIGHT, InputActionID::TOGGLE_CURSOR);
    userInputComponent.bindMouseButton(MOUSE_BUTTON_LEFT, InputActionID::MOVE_TO_TARGET);
}

void UserInputSystem::takeInput(
    UserInputComponent& userInputComponent,
    bool isCursorActive)
{
    if (userInputComponent.takeInputKey())
    {
        return;
    }

    if (userInputComponent.takeInputMouse(isCursorActive))
    {
        return;
    }

    else if (!isCursorActive)
    {
        userInputComponent.takeInputGesture();
    }
}

void UserInputSystem::triggerAction(
    UserInputComponent& userInputComponent,
    Hero& hero,
    Cursor const& cursor,
    Map const& map,
    GameCamera const& gameCamera)
{
    if (!hero.energy.isReady())
    {
        return;
    }

    if (userInputComponent.inputAction() == InputActionID::NONE)
    {
        //* Trigger input agnostic actions, eg. non-empty path
        hero.movement.trigger();

        return;
    }

    switch (userInputComponent.inputAction())
    {
        case InputActionID::ACT_UP:
        {
            if (map.enemies.ids.contains(
                    Vector2Add(
                        hero.position.tilePosition(),
                        Directions::V_UP)))
            {
                snx::debug::cliLog("Hero would attack\n");
                hero.energy.consume();
                break;
            }

            hero.movement.trigger(
                Directions::V_UP);

            break;
        }

        case InputActionID::ACT_LEFT:
        {
            if (map.enemies.ids.contains(
                    Vector2Add(
                        hero.position.tilePosition(),
                        Directions::V_LEFT)))
            {
                snx::debug::cliLog("Hero would attack\n");
                hero.energy.consume();
                break;
            }

            hero.movement.trigger(
                Directions::V_LEFT);

            break;
        }

        case InputActionID::ACT_DOWN:
        {
            if (map.enemies.ids.contains(
                    Vector2Add(
                        hero.position.tilePosition(),
                        Directions::V_DOWN)))
            {
                snx::debug::cliLog("Hero would attack\n");
                hero.energy.consume();
                break;
            }

            hero.movement.trigger(
                Directions::V_DOWN);

            break;
        }

        case InputActionID::ACT_RIGHT:
        {
            if (map.enemies.ids.contains(
                    Vector2Add(
                        hero.position.tilePosition(),
                        Directions::V_RIGHT)))
            {
                snx::debug::cliLog("Hero would attack\n");
                hero.energy.consume();
                break;
            }

            hero.movement.trigger(
                Directions::V_RIGHT);

            break;
        }

        case InputActionID::MOVE_TO_TARGET:
        {
            hero.movement.trigger(PathfinderSystem::findPath(
                map,
                hero.position.tilePosition(),
                cursor.position.tilePosition(),
                gameCamera));

            break;
        }

        case InputActionID::ACT_IN_PLACE:
        {
            Vector2I heroTilePosition{hero.position.tilePosition()};

            //* Wait if nothing to interact
            if (!map.objects.events.contains(heroTilePosition))
            {
                snx::Logger::log("Wait...");
                hero.energy.consume();
                break;
            }

            snx::PublisherStatic::publish(map.objects.events.at(heroTilePosition));

            break;
        }

        case InputActionID::TOGGLE_CURSOR:
        {
            snx::PublisherStatic::publish(Event::CURSOR_TOGGLE);

            break;
        }

        default:
            break;
    }

    //* Reset
    userInputComponent.resetInputAction();
}
