#include "UserInputSystem.h"

//* #define DEBUG_GESTURES
//* #define DEBUG_GESTURE_EVENTS

#include "Cursor.h"
#include "DamageSystem.h"
#include "Directions.h"
#include "EventId.h"
#include "GameCamera.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "InputActionId.h"
#include "Logger.h"
#include "Map.h"
#include "PathfinderSystem.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "UserInput.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void UserInputSystem::setDefaultInputMappings(
    UserInput& userInputComponent)
{
    userInputComponent.bindKey(KEY_NULL, InputActionId::NONE);

    userInputComponent.bindKey(KEY_W, InputActionId::ACT_UP);
    userInputComponent.bindKey(KEY_K, InputActionId::ACT_UP);
    userInputComponent.bindKey(KEY_UP, InputActionId::ACT_UP);

    userInputComponent.bindKey(KEY_A, InputActionId::ACT_LEFT);
    userInputComponent.bindKey(KEY_K, InputActionId::ACT_LEFT);
    userInputComponent.bindKey(KEY_LEFT, InputActionId::ACT_LEFT);

    userInputComponent.bindKey(KEY_S, InputActionId::ACT_DOWN);
    userInputComponent.bindKey(KEY_J, InputActionId::ACT_DOWN);
    userInputComponent.bindKey(KEY_DOWN, InputActionId::ACT_DOWN);

    userInputComponent.bindKey(KEY_D, InputActionId::ACT_RIGHT);
    userInputComponent.bindKey(KEY_L, InputActionId::ACT_RIGHT);
    userInputComponent.bindKey(KEY_RIGHT, InputActionId::ACT_RIGHT);

    userInputComponent.bindKey(KEY_SPACE, InputActionId::ACT_IN_PLACE);

    userInputComponent.bindModifierKey(KEY_LEFT_SHIFT, InputActionId::MOD);

    userInputComponent.bindMouseButton(MOUSE_BUTTON_RIGHT, InputActionId::TOGGLE_CURSOR);
    userInputComponent.bindMouseButton(MOUSE_BUTTON_LEFT, InputActionId::MOVE_TO_TARGET);
}

void UserInputSystem::takeInput(
    UserInput& userInputComponent,
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

    hero.energy.consume();
}

void UserInputSystem::triggerAction(
    UserInput& userInputComponent,
    Hero& hero,
    Cursor const& cursor,
    Map& map,
    GameCamera const& gameCamera)
{
    if (!hero.energy.isReady())
    {
        return;
    }

    if (userInputComponent.inputAction() == InputActionId::NONE)
    {
        //* Trigger input agnostic actions, eg. non-empty path
        hero.transform.trigger();

        return;
    }

    switch (userInputComponent.inputAction())
    {
        case InputActionId::ACT_UP:
        {
            Vector2I target{
                Vector2Add(
                    hero.position.tilePosition(),
                    Directions::up)};

            if (map.enemies.ids.contains(target))
            {
                performAttack(hero, map, target);

                break;
            }

            hero.transform.trigger(
                Directions::up);

            break;
        }

        case InputActionId::ACT_LEFT:
        {
            Vector2I target{
                Vector2Add(
                    hero.position.tilePosition(),
                    Directions::left)};

            if (map.enemies.ids.contains(target))
            {
                performAttack(hero, map, target);

                break;
            }

            hero.transform.trigger(
                Directions::left);

            break;
        }

        case InputActionId::ACT_DOWN:
        {
            Vector2I target{
                Vector2Add(
                    hero.position.tilePosition(),
                    Directions::down)};

            if (map.enemies.ids.contains(target))
            {
                performAttack(hero, map, target);

                break;
            }

            hero.transform.trigger(
                Directions::down);

            break;
        }

        case InputActionId::ACT_RIGHT:
        {
            Vector2I target{
                Vector2Add(
                    hero.position.tilePosition(),
                    Directions::right)};

            if (map.enemies.ids.contains(target))
            {
                performAttack(hero, map, target);

                break;
            }

            hero.transform.trigger(
                Directions::right);

            break;
        }

        case InputActionId::MOVE_TO_TARGET:
        {
            trigger(
                &hero.transform,
                PathfinderSystem::findPath(
                    map,
                    hero.position.tilePosition(),
                    cursor.position.tilePosition(),
                    gameCamera));

            break;
        }

        case InputActionId::ACT_IN_PLACE:
        {
            Vector2I heroTilePosition{hero.position.tilePosition()};

            //* Wait if nothing to interact
            if (!map.objects.events.contains(heroTilePosition))
            {
                snx::Logger::log("Hero waits...");

                hero.energy.consume();

                regenerate(&hero.health);

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
    userInputComponent.resetInputAction();
}
