#include "InputHandler.h"
#include <string>
#define DEBUG_GESTURES

#include "Cursor.h"
#include "Debugger.h"
#include "Directions.h"
#include "GameCamera.h"
#include "Hero.h"
#include "InputActionID.h"
#include "Logger.h"
#include "Map.h"
#include "Pathfinder.h"
#include "Position.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <utility>

void InputHandler::bindKey(int key, InputActionID action)
{
    keyToInputActionID_.insert(std::make_pair(key, action));
}

void InputHandler::bindMouseButton(int key, InputActionID action)
{
    mouseButtonToInputActionID_.insert(std::make_pair(key, action));
}

void InputHandler::bindModifierKey(int key, InputActionID action)
{
    inputActionIDToModifierKey_.insert(std::make_pair(action, key));
}

void InputHandler::setDefaultInputMappings()
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

    bindMouseButton(MOUSE_BUTTON_LEFT, InputActionID::moveToTarget);
}

bool InputHandler::takeInputMouse()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        inputAction_ = mouseButtonToInputActionID_[MOUSE_BUTTON_RIGHT];
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        inputAction_ = mouseButtonToInputActionID_[MOUSE_BUTTON_LEFT];
    }

    // Check if input is invalid (need exception for mouse toggle action)
    if (inputAction_ == InputActionID::none)
    {

        return false;
    }

    return true;
}

bool InputHandler::takeInputKey()
{
    // Update key pressed
    // Set lastKey only to valid inputs (associated with actions)
    lastKey_ = (currentKey_) ? currentKey_ : lastKey_;
    currentKey_ = GetKeyPressed();

    // Check modifiers
    modifier_ = IsKeyDown(inputActionIDToModifierKey_[InputActionID::mod]);

    // Repeat last key if no input but modifier down
#if defined(TERMUX)
    if ((modifier_ && !currentKey))
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

bool InputHandler::takeInputGesture()
{
    // Update gesture (ignore GESTURE_NONE for lastGesture)
    lastGesture_ = currentGesture_;
    currentGesture_ = GetGestureDetected();
#if defined(DEBUG) && defined(DEBUG_GESTURES)
    snx::debug::cliLog(std::to_string(currentGesture_) + "\n");
#endif

    // Update hold time
    if (touchUpTime_ < touchDownTime_)
    {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
        snx::debug::cliLog("down " + std::to_string(touchDownTime_) + ", up " + std::to_string(touchUpTime_) + "\n");
#endif
        touchHoldDuration_ = GetTime() - touchDownTime_;
    }

    // Detect gesture change
    if (currentGesture_ != lastGesture_)
    {
        switch (currentGesture_)
        {
            default:
            case GESTURE_NONE:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::debug::cliLog("Triggered GESTURE_NONE\n");
#endif
                if (lastGesture_ != GESTURE_NONE)
                {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                    snx::debug::cliLog("Triggered TOUCH UP\n");
#endif
                    touchUpTime_ = GetTime();
                }

                // Check for Tap event
                if ((touchUpTime_ - touchDownTime_) < maxTapTime)
                {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                    snx::Logger::log("Triggered TAP EVENT\n");
                    snx::debug::cliLog("Triggered TAP EVENT\n");
#endif
                    inputAction_ = InputActionID::actInPlace;
                    break;
                }

                inputAction_ = InputActionID::none;

                break;
            }

            case GESTURE_TAP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::debug::cliLog("Triggered GESTURE_TAP (TOUCH DOWN)\n");
#endif
                snx::debug::cliLog("Triggered GESTURE_TAP (TOUCH DOWN)\n");
                touchDownTime_ = GetTime();

                break;
            }

            case GESTURE_DOUBLETAP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_DOUBLETAP\n");
                snx::debug::cliLog("Triggered GESTURE_DOUBLETAP\n");
#endif
                inputAction_ = InputActionID::actInPlace;

                break;
            }

            case GESTURE_HOLD:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::debug::cliLog("Triggered GESTURE_HOLD\n");
#endif
                if ((touchUpTime_ - touchDownTime_) > minHoldTime)
                {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                    snx::Logger::log("Triggered HOLD EVENT\n");
                    snx::debug::cliLog("Triggered HOLD EVENT\n");
#endif
                    // Trigger Hold: move cursor to get info about tile
                }

                break;
            }

            case GESTURE_DRAG:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_DRAG\n");
                snx::debug::cliLog("Triggered GESTURE_DRAG\n");
#endif
                // Set modifier
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

    if (inputAction_ == InputActionID::none)
    {
        return false;
    }

    return true;
}

void InputHandler::takeInput()
{
    if (takeInputKey())
    {
        return;
    }

    if (takeInputMouse())
    {
        return;
    }

    else
    {
        takeInputGesture();
    }
}

void InputHandler::triggerAction(
    Hero& hero,
    Cursor const& cursor,
    Map const& map,
    GameCamera const& gameCamera)
{
    if (!hero.energy().isReady())
    {
        return;
    }

    if (inputAction_ == InputActionID::none)
    {
        // Trigger input agnostic actions, eg. non-empty path
        hero.movement().trigger();

        return;
    }

    switch (inputAction_)
    {
        case InputActionID::actUp:
        {
            if (map.enemies_.positions().contains(
                    Position{
                        Vector2Add(
                            hero.position().tilePosition(),
                            Directions::V_UP)}))
            {
                snx::debug::cliLog("Hero would attack\n");
                hero.energy().consume();
                break;
            }

            hero.movement().trigger(
                Directions::V_UP);

            break;
        }

        case InputActionID::actLeft:
        {
            if (map.enemies_.positions().contains(
                    Position{
                        Vector2Add(
                            hero.position().tilePosition(),
                            Directions::V_LEFT)}))
            {
                snx::debug::cliLog("Hero would attack\n");
                hero.energy().consume();
                break;
            }

            hero.movement().trigger(
                Directions::V_LEFT);

            break;
        }

        case InputActionID::actDown:
        {
            if (map.enemies_.positions().contains(
                    Position{
                        Vector2Add(
                            hero.position().tilePosition(),
                            Directions::V_DOWN)}))
            {
                snx::debug::cliLog("Hero would attack\n");
                hero.energy().consume();
                break;
            }

            hero.movement().trigger(
                Directions::V_DOWN);

            break;
        }

        case InputActionID::actRight:
        {
            if (map.enemies_.positions().contains(
                    Position{
                        Vector2Add(
                            hero.position().tilePosition(),
                            Directions::V_RIGHT)}))
            {
                snx::debug::cliLog("Hero would attack\n");
                hero.energy().consume();
                break;
            }

            hero.movement().trigger(
                Directions::V_RIGHT);

            break;
        }

        case InputActionID::moveToTarget:
        {
            hero.movement().trigger(Pathfinder::findPath(
                map,
                hero.position().tilePosition(),
                cursor.position_.tilePosition(),
                gameCamera));

            break;
        }

        case InputActionID::actInPlace:
        {
            Vector2I heroTilePosition{hero.position().tilePosition()};

            // Wait if nothing to interact
            if (!map.objects_.events().contains(heroTilePosition))
            {
                snx::Logger::log("Wait...");
                hero.energy().consume();
                break;
            }

            snx::PublisherStatic::publish(map.objects_.event(heroTilePosition));

            break;
        }

        default:
            break;
    }

    // Reset
    inputAction_ = InputActionID::none;
}
