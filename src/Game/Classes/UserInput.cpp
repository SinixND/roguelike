#include "UserInput.h"
#include "EventId.h"
#include "InputActionId.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>
#include <utility>

//* Maximum hold time for tap event to trigger
double constexpr maxTapTime{0.3f};
//* Minimum hold time for hold event to trigger
double constexpr minHoldTime{0.3f};
//* Maximum time between taps for double tap event to trigger
double constexpr maxDoubleTapTime{0.3f};

void bindKey(
    UserInput* inputData,
    int key,
    InputActionId action)
{
    inputData->keyToInputActionId_.insert(std::make_pair(key, action));
}

void bindMouseButton(
    UserInput* inputData,
    int key,
    InputActionId action)
{
    inputData->mouseButtonToInputActionId_.insert(std::make_pair(key, action));
}

void bindModifierKey(
    UserInput* inputData,
    int key,
    InputActionId action)
{
    inputData->inputActionIdToModifierKey_.insert(std::make_pair(action, key));
}

bool takeInputMouse(
    UserInput* inputData,
    bool isCursorActive)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        inputData->inputAction_ = inputData->mouseButtonToInputActionId_[MOUSE_BUTTON_RIGHT];
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        inputData->inputAction_ = inputData->mouseButtonToInputActionId_[MOUSE_BUTTON_LEFT];
    }

    //* Check if input is invalid (need exception for mouse toggle action)
    if (inputData->inputAction_ == InputActionId::NONE
        || (!(inputData->inputAction_ == InputActionId::TOGGLE_CURSOR)
            && !isCursorActive))
    {

        return false;
    }

    return true;
}

bool takeInputKey(UserInput* inputData)
{
    //* Update key pressed
    //* Set lastKey only to valid inputs (associated with actions)
    inputData->lastKey_ = (inputData->currentKey_) ? inputData->currentKey_ : inputData->lastKey_;
    inputData->currentKey_ = GetKeyPressed();

    //* Check modifiers
    inputData->modifier_ = IsKeyDown(inputData->inputActionIdToModifierKey_[InputActionId::MOD]);

    //* Repeat last key if no input but modifier down
#if defined(TERMUX)
    if ((inputData->modifier_ && !inputData->currentKey_))
#else
    if ((inputData->modifier_ && !inputData->currentKey_) || IsKeyPressedRepeat(inputData->lastKey_))
#endif
    {
        inputData->currentKey_ = inputData->lastKey_;
    }

    inputData->inputAction_ = inputData->keyToInputActionId_[inputData->currentKey_];

    if (inputData->inputAction_ == InputActionId::NONE)
    {
        return false;
    }

    return true;
}

bool takeInputGesture(UserInput* inputData)
{
    //* IMPORTANT NOTE:
    //* Implemented events TAP, DOUBLETAP and HOLD as raylib gesture registration was unreliable
    //* Therefore disabled any functionality of GESTURE_TAP and GESTURE_DOUBLE_TAP
    //* but left in game logging so it gets noticed when it was fixed

    //* Update gestures
    inputData->lastGesture_ = inputData->currentGesture_;
    inputData->currentGesture_ = GetGestureDetected();

    //* Detect gesture change
    if (inputData->currentGesture_ != inputData->lastGesture_)
    {
        switch (inputData->currentGesture_)
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
                inputData->touchUpTime_ = GetTime();

                //* Reset hold duration
                inputData->touchHoldDuration_ = 0;

                //* Check for Tap events
                if (inputData->lastGesture_ == GESTURE_HOLD
                    && (inputData->touchUpTime_ - inputData->touchDownTime_) < maxTapTime)
                {
                    //* Check for double tap
                    if ((inputData->touchUpTime_ - inputData->lastTap_) < maxDoubleTapTime)
                    {
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                        snx::Logger::log("Triggered DOUBLE TAP EVENT\n");
                        snx::debug::cliLog("Triggered DOUBLE TAP EVENT\n");
#endif
                        inputData->inputAction_ = InputActionId::ACT_IN_PLACE;
                    }
                    else
                    {
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                        snx::Logger::log("Triggered TAP EVENT\n");
                        snx::debug::cliLog("Triggered TAP EVENT\n");
#endif
                        // inputAction_ = InputActionId::actInPlace;
                    }

                    inputData->lastTap_ = inputData->touchUpTime_;

                    break;
                }

                inputData->inputAction_ = InputActionId::NONE;

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
                // inputAction_ = InputActionId::actInPlace;

                break;
            }

            case GESTURE_HOLD:
            {
#if defined(DEBUG) && (defined(DEBUG_GESTURES) || defined(DEBUG_GESTURE_EVENTS))
                snx::Logger::log("Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n");
                snx::debug::cliLog("Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n");
#endif
                inputData->touchDownTime_ = GetTime();

                break;
            }

            case GESTURE_DRAG:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered first GESTURE_DRAG\n");
                snx::debug::cliLog("Triggered first GESTURE_DRAG\n");
#endif
                //* Set modifier
                inputData->modifier_ = true;

                Vector2 direction = Vector2MainDirection(GetGestureDragVector());
                if (direction == Vector2{0, -1})
                {
                    inputData->inputAction_ = InputActionId::ACT_UP;
                }

                else if (direction == Vector2{-1, 0})
                {
                    inputData->inputAction_ = InputActionId::ACT_LEFT;
                }

                else if (direction == Vector2{0, 1})
                {
                    inputData->inputAction_ = InputActionId::ACT_DOWN;
                }

                else if (direction == Vector2{1, 0})
                {
                    inputData->inputAction_ = InputActionId::ACT_RIGHT;
                }

                break;
            }

            case GESTURE_SWIPE_UP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_UP\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_UP\n");
#endif
                inputData->inputAction_ = InputActionId::ACT_UP;

                break;
            }

            case GESTURE_SWIPE_LEFT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_LEFT\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_LEFT\n");
#endif
                inputData->inputAction_ = InputActionId::ACT_LEFT;

                break;
            }

            case GESTURE_SWIPE_DOWN:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_DOWN\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_DOWN\n");
#endif
                inputData->inputAction_ = InputActionId::ACT_DOWN;

                break;
            }

            case GESTURE_SWIPE_RIGHT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_RIGHT\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_RIGHT\n");
#endif
                inputData->inputAction_ = InputActionId::ACT_RIGHT;

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
        switch (inputData->currentGesture_)
        {
            default:
            case GESTURE_NONE:
            {
                inputData->inputAction_ = InputActionId::NONE;

                break;
            }

            case GESTURE_HOLD:
            {
                inputData->touchHoldDuration_ = GetTime() - inputData->touchDownTime_;

                if ((inputData->touchHoldDuration_) > minHoldTime)
                {
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                    snx::Logger::log("Triggered HOLD EVENT\n");
                    snx::debug::cliLog("Triggered HOLD EVENT\n");
#endif
                    //* Get/Set info panel reference to tile/object/enemy at current position
                    snx::PublisherStatic::publish(EventId::CURSOR_POSITION__CHANGED);
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
                inputData->modifier_ = true;

                Vector2 direction = Vector2MainDirection(GetGestureDragVector());
                if (direction == Vector2{0, -1})
                {
                    inputData->inputAction_ = InputActionId::ACT_UP;
                }

                else if (direction == Vector2{-1, 0})
                {
                    inputData->inputAction_ = InputActionId::ACT_LEFT;
                }

                else if (direction == Vector2{0, 1})
                {
                    inputData->inputAction_ = InputActionId::ACT_DOWN;
                }

                else if (direction == Vector2{1, 0})
                {
                    inputData->inputAction_ = InputActionId::ACT_RIGHT;
                }

                break;
            }
        }
    }

    if (inputData->inputAction_ == InputActionId::NONE)
    {
        return false;
    }

    return true;
}
