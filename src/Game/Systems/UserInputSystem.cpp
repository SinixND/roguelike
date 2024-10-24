#include "UserInputSystem.h"
#include <string>
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
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <utility>

void UserInputSystem::bindKey(int key, InputActionID action)
{
    keyToInputActionID_.insert(std::make_pair(key, action));
}

void UserInputSystem::bindMouseButton(int key, InputActionID action)
{
    mouseButtonToInputActionID_.insert(std::make_pair(key, action));
}

void UserInputSystem::bindModifierKey(int key, InputActionID action)
{
    inputActionIDToModifierKey_.insert(std::make_pair(action, key));
}

void UserInputSystem::setDefaultInputMappings()
{
    bindKey(KEY_NULL, InputActionID::none);

    bindKey(KEY_W, InputActionID::actUp);
    bindKey(KEY_K, InputActionID::actUp);
    bindKey(KEY_UP, InputActionID::actUp);

    bindKey(KEY_A, InputActionID::actLeft);
    bindKey(KEY_K, InputActionID::actLeft);
    bindKey(KEY_LEFT, InputActionID::actLeft);

    bindKey(KEY_S, InputActionID::actDown);
    bindKey(KEY_J, InputActionID::actDown);
    bindKey(KEY_DOWN, InputActionID::actDown);

    bindKey(KEY_D, InputActionID::actRight);
    bindKey(KEY_L, InputActionID::actRight);
    bindKey(KEY_RIGHT, InputActionID::actRight);

    bindKey(KEY_SPACE, InputActionID::actInPlace);

    bindModifierKey(KEY_LEFT_SHIFT, InputActionID::mod);

    bindMouseButton(MOUSE_BUTTON_RIGHT, InputActionID::toggleCursor);
    bindMouseButton(MOUSE_BUTTON_LEFT, InputActionID::moveToTarget);
}

bool UserInputSystem::takeInputMouse(bool isCursorActive)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        inputAction_ = mouseButtonToInputActionID_[MOUSE_BUTTON_RIGHT];
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        inputAction_ = mouseButtonToInputActionID_[MOUSE_BUTTON_LEFT];
    }

    //* Check if input is invalid (need exception for mouse toggle action)
    if (inputAction_ == InputActionID::none
        || (!(inputAction_ == InputActionID::toggleCursor)
            && !isCursorActive))
    {

        return false;
    }

    return true;
}

bool UserInputSystem::takeInputKey()
{
    //* Update key pressed
    //* Set lastKey only to valid inputs (associated with actions)
    lastKey_ = (currentKey_) ? currentKey_ : lastKey_;
    currentKey_ = GetKeyPressed();

    //* Check modifiers
    modifier_ = IsKeyDown(inputActionIDToModifierKey_[InputActionID::mod]);

    //* Repeat last key if no input but modifier down
#if defined(TERMUX)
    if ((modifier_ && !currentKey_))
#else
    if ((modifier_ && !currentKey_) || IsKeyPressedRepeat(lastKey_))
#endif
    {
        currentKey_ = lastKey_;
    }

    inputAction_ = keyToInputActionID_[currentKey_];

    if (inputAction_ == InputActionID::none)
    {
        return false;
    }

    return true;
}

bool UserInputSystem::takeInputGesture()
{
    //* IMPORTANT NOTE:
    //* Implemented events TAP, DOUBLETAP and HOLD as raylib gesture registration was unreliable
    //* Therefore disabled any functionality of GESTURE_TAP and GESTURE_DOUBLE_TAP
    //* but left in game logging so it gets noticed when it was fixed

    //* Update gestures
    lastGesture_ = currentGesture_;
    currentGesture_ = GetGestureDetected();

    //* Detect gesture change
    if (currentGesture_ != lastGesture_)
    {
        switch (currentGesture_)
        {
            default:
            case GESTURE_NONE:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_NONE\n");
                snx::debug::cliLog("Triggered GESTURE_NONE\n");
#endif
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                snx::Logger::log("Triggered TOUCH UP EVENT\n");
                snx::debug::cliLog("Triggered TOUCH UP EVENT\n");
#endif
                touchUpTime_ = GetTime();

                //* Reset hold duration
                touchHoldDuration_ = 0;

                //* Check for Tap events
                if (lastGesture_ == GESTURE_HOLD
                    && (touchUpTime_ - touchDownTime_) < maxTapTime)
                {
                    //* Check for double tap
                    if ((touchUpTime_ - lastTap_) < maxDoubleTapTime)
                    {
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                        snx::Logger::log("Triggered DOUBLE TAP EVENT\n");
                        snx::debug::cliLog("Triggered DOUBLE TAP EVENT\n");
#endif
                        inputAction_ = InputActionID::actInPlace;
                    }
                    else
                    {
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                        snx::Logger::log("Triggered TAP EVENT\n");
                        snx::debug::cliLog("Triggered TAP EVENT\n");
#endif
                        // inputAction_ = InputActionID::actInPlace;
                    }

                    lastTap_ = touchUpTime_;

                    break;
                }

                inputAction_ = InputActionID::none;

                break;
            }

            case GESTURE_TAP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_TAP\n");
#endif
                // touchDownTime_ = GetTime();

                break;
            }

            case GESTURE_DOUBLETAP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_DOUBLETAP\n");
#endif
                // inputAction_ = InputActionID::actInPlace;

                break;
            }

            case GESTURE_HOLD:
            {
#if defined(DEBUG) && (defined(DEBUG_GESTURES) || defined(DEBUG_GESTURE_EVENTS))
                snx::Logger::log("Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n");
                snx::debug::cliLog("Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n");
#endif
                touchDownTime_ = GetTime();

                break;
            }

            case GESTURE_DRAG:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered first GESTURE_DRAG\n");
                snx::debug::cliLog("Triggered first GESTURE_DRAG\n");
#endif
                //* Set modifier
                modifier_ = true;

                Vector2 direction = Vector2MainDirection(GetGestureDragVector());
                if (direction == Vector2{0, -1})
                {
                    inputAction_ = InputActionID::actUp;
                }

                else if (direction == Vector2{-1, 0})
                {
                    inputAction_ = InputActionID::actLeft;
                }

                else if (direction == Vector2{0, 1})
                {
                    inputAction_ = InputActionID::actDown;
                }

                else if (direction == Vector2{1, 0})
                {
                    inputAction_ = InputActionID::actRight;
                }

                break;
            }

            case GESTURE_SWIPE_UP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_UP\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_UP\n");
#endif
                inputAction_ = InputActionID::actUp;

                break;
            }

            case GESTURE_SWIPE_LEFT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_LEFT\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_LEFT\n");
#endif
                inputAction_ = InputActionID::actLeft;

                break;
            }

            case GESTURE_SWIPE_DOWN:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_DOWN\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_DOWN\n");
#endif
                inputAction_ = InputActionID::actDown;

                break;
            }

            case GESTURE_SWIPE_RIGHT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_RIGHT\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_RIGHT\n");
#endif
                inputAction_ = InputActionID::actRight;

                break;
            }

            case GESTURE_PINCH_IN:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_PINCH_IN\n");
                snx::debug::cliLog("Triggered GESTURE_PINCH_IN\n");
#endif

                break;
            }

            case GESTURE_PINCH_OUT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_PINCH_OUT\n");
                snx::debug::cliLog("Triggered GESTURE_PINCH_OUT\n");
#endif

                break;
            }
        }
    }

    //* Detect continuous gestures
    //* if (currentGesture_ == lastGesture_)
    else
    {
        switch (currentGesture_)
        {
            default:
            case GESTURE_NONE:
            {
                inputAction_ = InputActionID::none;

                break;
            }

            case GESTURE_HOLD:
            {
                touchHoldDuration_ = GetTime() - touchDownTime_;

                if ((touchHoldDuration_) > minHoldTime)
                {
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                    snx::Logger::log("Triggered HOLD EVENT\n");
                    snx::debug::cliLog("Triggered HOLD EVENT\n");
#endif
                    //* Get/Set info panel reference to tile/object/enemy at current position
                    snx::PublisherStatic::publish(Event::cursorPositionChanged);
                }

                break;
            }

            case GESTURE_DRAG:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_DRAG\n");
                snx::debug::cliLog("Triggered GESTURE_DRAG\n");
#endif
                //* Set modifier
                modifier_ = true;

                Vector2 direction = Vector2MainDirection(GetGestureDragVector());
                if (direction == Vector2{0, -1})
                {
                    inputAction_ = InputActionID::actUp;
                }

                else if (direction == Vector2{-1, 0})
                {
                    inputAction_ = InputActionID::actLeft;
                }

                else if (direction == Vector2{0, 1})
                {
                    inputAction_ = InputActionID::actDown;
                }

                else if (direction == Vector2{1, 0})
                {
                    inputAction_ = InputActionID::actRight;
                }

                break;
            }
        }
    }

    if (inputAction_ == InputActionID::none)
    {
        return false;
    }

    return true;
}

void UserInputSystem::takeInput(bool isCursorActive)
{
    if (takeInputKey())
    {
        return;
    }

    if (takeInputMouse(isCursorActive))
    {
        return;
    }

    else if (!isCursorActive)
    {
        takeInputGesture();
    }
}

void UserInputSystem::triggerAction(
    Hero& hero,
    Cursor const& cursor,
    Map const& map,
    GameCamera const& gameCamera)
{
    if (!hero.energy.isReady())
    {
        return;
    }

    if (inputAction_ == InputActionID::none)
    {
        //* Trigger input agnostic actions, eg. non-empty path
        hero.movement.trigger();

        return;
    }

    switch (inputAction_)
    {
        case InputActionID::actUp:
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

        case InputActionID::actLeft:
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

        case InputActionID::actDown:
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

        case InputActionID::actRight:
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

        case InputActionID::moveToTarget:
        {
            hero.movement.trigger(PathfinderSystem::findPath(
                map,
                hero.position.tilePosition(),
                cursor.position.tilePosition(),
                gameCamera));

            break;
        }

        case InputActionID::actInPlace:
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

        case InputActionID::toggleCursor:
        {
            snx::PublisherStatic::publish(Event::cursorToggle);

            break;
        }

        default:
            break;
    }

    //* Reset
    inputAction_ = InputActionID::none;
}
