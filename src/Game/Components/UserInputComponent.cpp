#include "UserInputComponent.h"
#include "EventId.h"
#include "InputActionID.h"
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

void UserInputComponent::bindKey(int key, InputActionID action)
{
    keyToInputActionID_.insert(std::make_pair(key, action));
}

void UserInputComponent::bindMouseButton(int key, InputActionID action)
{
    mouseButtonToInputActionID_.insert(std::make_pair(key, action));
}

void UserInputComponent::bindModifierKey(int key, InputActionID action)
{
    inputActionIDToModifierKey_.insert(std::make_pair(action, key));
}

bool UserInputComponent::takeInputMouse(bool isCursorActive)
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
    if (inputAction_ == InputActionID::NONE
        || (!(inputAction_ == InputActionID::TOGGLE_CURSOR)
            && !isCursorActive))
    {

        return false;
    }

    return true;
}

bool UserInputComponent::takeInputKey()
{
    //* Update key pressed
    //* Set lastKey only to valid inputs (associated with actions)
    lastKey_ = (currentKey_) ? currentKey_ : lastKey_;
    currentKey_ = GetKeyPressed();

    //* Check modifiers
    modifier_ = IsKeyDown(inputActionIDToModifierKey_[InputActionID::MOD]);

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

    if (inputAction_ == InputActionID::NONE)
    {
        return false;
    }

    return true;
}

bool UserInputComponent::takeInputGesture()
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
                        inputAction_ = InputActionID::ACT_IN_PLACE;
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

                inputAction_ = InputActionID::NONE;

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
                    inputAction_ = InputActionID::ACT_UP;
                }

                else if (direction == Vector2{-1, 0})
                {
                    inputAction_ = InputActionID::ACT_LEFT;
                }

                else if (direction == Vector2{0, 1})
                {
                    inputAction_ = InputActionID::ACT_DOWN;
                }

                else if (direction == Vector2{1, 0})
                {
                    inputAction_ = InputActionID::ACT_RIGHT;
                }

                break;
            }

            case GESTURE_SWIPE_UP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_UP\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_UP\n");
#endif
                inputAction_ = InputActionID::ACT_UP;

                break;
            }

            case GESTURE_SWIPE_LEFT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_LEFT\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_LEFT\n");
#endif
                inputAction_ = InputActionID::ACT_LEFT;

                break;
            }

            case GESTURE_SWIPE_DOWN:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_DOWN\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_DOWN\n");
#endif
                inputAction_ = InputActionID::ACT_DOWN;

                break;
            }

            case GESTURE_SWIPE_RIGHT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_RIGHT\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_RIGHT\n");
#endif
                inputAction_ = InputActionID::ACT_RIGHT;

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
                inputAction_ = InputActionID::NONE;

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
                    snx::PublisherStatic::publish(EventId::CURSOR_POSITION_CHANGED);
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
                    inputAction_ = InputActionID::ACT_UP;
                }

                else if (direction == Vector2{-1, 0})
                {
                    inputAction_ = InputActionID::ACT_LEFT;
                }

                else if (direction == Vector2{0, 1})
                {
                    inputAction_ = InputActionID::ACT_DOWN;
                }

                else if (direction == Vector2{1, 0})
                {
                    inputAction_ = InputActionID::ACT_RIGHT;
                }

                break;
            }
        }
    }

    if (inputAction_ == InputActionID::NONE)
    {
        return false;
    }

    return true;
}

InputActionID UserInputComponent::inputAction() const
{
    return inputAction_;
}

void UserInputComponent::resetInputAction()
{
    inputAction_ = InputActionID::NONE;
}