#include "UserInputSystem.h"

//* #define DEBUG_GESTURES
//* #define DEBUG_GESTURE_EVENTS

#include "Convert.h"
#include "Cursor.h"
#include "DamageSystem.h"
#include "Directions.h"
#include "EnergyComponent.h"
#include "EventId.h"
#include "GameCamera.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "InputActionId.h"
#include "Logger.h"
#include "Map.h"
#include "PathfinderSystem.h"
#include "PublisherStatic.h"
#include "UserInput.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void UserInputSystem::setDefaultInputMappings(
    UserInput& userInput)
{
    bindKey(
        &userInput,
        KEY_NULL,
        InputActionId::NONE);

    bindKey(
        &userInput,
        KEY_W,
        InputActionId::ACT_UP);

    bindKey(
        &userInput,
        KEY_K,
        InputActionId::ACT_UP);

    bindKey(
        &userInput,
        KEY_UP,
        InputActionId::ACT_UP);

    bindKey(
        &userInput,
        KEY_A,
        InputActionId::ACT_LEFT);

    bindKey(
        &userInput,
        KEY_K,
        InputActionId::ACT_LEFT);

    bindKey(
        &userInput,
        KEY_LEFT,
        InputActionId::ACT_LEFT);

    bindKey(
        &userInput,
        KEY_S,
        InputActionId::ACT_DOWN);

    bindKey(
        &userInput,
        KEY_J,
        InputActionId::ACT_DOWN);

    bindKey(
        &userInput,
        KEY_DOWN,
        InputActionId::ACT_DOWN);

    bindKey(
        &userInput,
        KEY_D,
        InputActionId::ACT_RIGHT);

    bindKey(
        &userInput,
        KEY_L,
        InputActionId::ACT_RIGHT);

    bindKey(
        &userInput,
        KEY_RIGHT,
        InputActionId::ACT_RIGHT);

    bindKey(
        &userInput,
        KEY_SPACE,
        InputActionId::ACT_IN_PLACE);

    bindModifierKey(
        &userInput,
        KEY_LEFT_SHIFT,
        InputActionId::MOD);

    bindMouseButton(
        &userInput,
        MOUSE_BUTTON_RIGHT,
        InputActionId::TOGGLE_CURSOR);

    bindMouseButton(
        &userInput,
        MOUSE_BUTTON_LEFT,
        InputActionId::MOVE_TO_TARGET);
}

void UserInputSystem::takeInput(
    UserInput& userInput,
    bool isCursorActive)
{
    if (takeInputKey(&userInput))
    {
        return;
    }

    if (takeInputMouse(
            &userInput,
            isCursorActive))
    {
        return;
    }

    else if (!isCursorActive)
    {
        takeInputGesture(&userInput);
    }
}

void performAttack(
    Hero& hero,
    Map& map,
    Vector2I const& target)
{
    snx::Logger::log("Hero deals ");

    DamageSystem::attack(
        hero.damage,
        map.enemies.healths.at(
            map.enemies.ids.at(
                target)));

    consume(&hero.energy);
}

void UserInputSystem::triggerAction(
    UserInput& userInput,
    Hero& hero,
    Cursor const& cursor,
    Map& map,
    GameCamera const& gameCamera)
{
    if (!hero.energy.isReady)
    {
        return;
    }

    if (userInput.inputAction_ == InputActionId::NONE)
    {
        //* Trigger input agnostic actions, eg. non-empty path
        triggerAuto(
            &hero.movement.direction_,
            &hero.movement.velocity_,
            &hero.movement.isTriggered_,
            hero.movement.speed_,
            &hero.movement.path_,
            hero.movement.isInProgress_);

        return;
    }

    switch (userInput.inputAction_)
    {
        case InputActionId::ACT_UP:
        {
            Vector2I target{
                Vector2Add(
                    Convert::worldToTile(hero.position),
                    Directions::up)};

            if (map.enemies.ids.contains(target))
            {
                performAttack(
                    hero,
                    map,
                    target);

                break;
            }

            triggerByDirection(
                &hero.movement.direction_,
                &hero.movement.velocity_,
                &hero.movement.isTriggered_,
                hero.movement.speed_,
                Directions::up);

            break;
        }

        case InputActionId::ACT_LEFT:
        {
            Vector2I target{
                Vector2Add(
                    Convert::worldToTile(hero.position),
                    Directions::left)};

            if (map.enemies.ids.contains(target))
            {
                performAttack(
                    hero,
                    map,
                    target);

                break;
            }

            triggerByDirection(
                &hero.movement.direction_,
                &hero.movement.velocity_,
                &hero.movement.isTriggered_,
                hero.movement.speed_,
                Directions::left);

            break;
        }

        case InputActionId::ACT_DOWN:
        {
            Vector2I target{
                Vector2Add(
                    Convert::worldToTile(hero.position),
                    Directions::down)};

            if (map.enemies.ids.contains(target))
            {
                performAttack(
                    hero,
                    map,
                    target);

                break;
            }

            triggerByDirection(
                &hero.movement.direction_,
                &hero.movement.velocity_,
                &hero.movement.isTriggered_,
                hero.movement.speed_,
                Directions::down);

            break;
        }

        case InputActionId::ACT_RIGHT:
        {
            Vector2I target{
                Vector2Add(
                    Convert::worldToTile(hero.position),
                    Directions::right)};

            if (map.enemies.ids.contains(target))
            {
                performAttack(
                    hero,
                    map,
                    target);

                break;
            }

            triggerByDirection(
                &hero.movement.direction_,
                &hero.movement.velocity_,
                &hero.movement.isTriggered_,
                hero.movement.speed_,
                Directions::right);

            break;
        }

        case InputActionId::MOVE_TO_TARGET:
        {
            triggerByPath(
                &hero.movement.direction_,
                &hero.movement.velocity_,
                &hero.movement.isTriggered_,
                hero.movement.speed_,
                &hero.movement.path_,
                PathfinderSystem::findPath(
                    map,
                    Convert::worldToTile(hero.position),
                    Convert::worldToTile(cursor.position),
                    gameCamera));

            break;
        }

        case InputActionId::ACT_IN_PLACE:
        {
            Vector2I heroTilePosition{Convert::worldToTile(hero.position)};

            //* Wait if nothing to interact
            if (!map.objects.events.contains(heroTilePosition))
            {
                snx::Logger::log("Hero waits...");

                consume(&hero.energy);

                regenerate(&hero.health);
                // hero.health.regenerate();

                break;
            }

            snx::PublisherStatic::publish(map.objects.events.at(heroTilePosition));

            break;
        }

        case InputActionId::TOGGLE_CURSOR:
        {
            snx::PublisherStatic::publish(EventId::CURSOR_TOGGLE);

            break;
        }

        default:
            break;
    }

    //* Reset
    userInput.inputAction_ = InputActionId::NONE;
}
