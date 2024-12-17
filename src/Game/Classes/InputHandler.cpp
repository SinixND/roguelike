#include "EventId.h"

#include "InputActionId.h"
#include "InputHandler.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>

//* Maximum hold time for tap event to trigger
double constexpr maxTapTime{0.3f};
//* Minimum hold time for hold event to trigger
double constexpr minHoldTime{0.3f};
//* Maximum time between taps for double tap event to trigger
double constexpr maxDoubleTapTime{0.3f};

InputActionId checkKeyboard(
    InputHandler& handler,
    InputMappings const& mappings)
{
    //* Update key pressed
    //* Set lastKey only to valid inputs (associated with actions)
    handler.lastKey = (handler.currentKey) ? handler.currentKey : handler.lastKey;
    handler.currentKey = GetKeyPressed();

    //* Check modifiers
    handler.modifier = IsKeyDown(mappings.modifierKey);

    //* Repeat last key if no input but modifier down
#if defined(TERMUX)
    if ((handler.modifier && !handler.currentKey))
#else
    if ((handler.modifier && !handler.currentKey) || IsKeyPressedRepeat(handler.lastKey))
#endif
    {
        handler.currentKey = handler.lastKey;
    }

    return mappings.keyboardToAction.at(handler.currentKey);
}

InputActionId checkMouse(
    InputMappings const& mappings,
    bool const isCursorActive)
{
    InputActionId inputAction{InputActionId::NONE};

    for (int mouseButton : {MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT})
    {
        if (
            (isCursorActive
             || mappings.mouseToAction.at(mouseButton) == InputActionId::TOGGLE_CURSOR)
            && IsMouseButtonPressed(mouseButton))
        {
            inputAction = mappings.mouseToAction.at(mouseButton);
        }
    }

    return inputAction;
}

InputActionId checkGesture(InputHandler& handler)
{
    InputActionId inputAction{InputActionId::NONE};

    //* IMPORTANT NOTE:
    //* Implemented events TAP, DOUBLETAP and HOLD as raylib gesture registration was unreliable
    //* Therefore disabled any functionality of GESTURE_TAP and GESTURE_DOUBLE_TAP
    //* but left in game logging so it gets noticed when it was fixed

    //* Update gestures
    handler.lastGesture = handler.currentGesture;
    handler.currentGesture = GetGestureDetected();

    //* Detect gesture change
    if (handler.currentGesture != handler.lastGesture)
    {
        switch (handler.currentGesture)
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
                handler.touchUpTime = GetTime();

                //* Reset hold duration
                handler.touchHoldDuration = 0;

                //* Check for Tap events
                if (handler.lastGesture == GESTURE_HOLD
                    && (handler.touchUpTime - handler.touchDownTime) < maxTapTime)
                {
                    //* Check for double tap
                    if ((handler.touchUpTime - handler.lastTap) < maxDoubleTapTime)
                    {
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                        snx::Logger::log("Triggered DOUBLE TAP EVENT\n");
                        snx::debug::cliLog("Triggered DOUBLE TAP EVENT\n");
#endif
                        inputAction = InputActionId::ACT_IN_PLACE;
                    }
                    else
                    {
#if defined(DEBUG) && defined(DEBUG_GESTURE_EVENTS)
                        snx::Logger::log("Triggered TAP EVENT\n");
                        snx::debug::cliLog("Triggered TAP EVENT\n");
#endif
                    }

                    handler.lastTap = handler.touchUpTime;

                    break;
                }

                inputAction = InputActionId::NONE;

                break;
            }

            case GESTURE_TAP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_TAP\n");
#endif
                // touchDownTime = GetTime();

                break;
            }

            case GESTURE_DOUBLETAP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_DOUBLETAP\n");
#endif
                // inputAction = InputActionId::actInPlace;

                break;
            }

            case GESTURE_HOLD:
            {
#if defined(DEBUG) && (defined(DEBUG_GESTURES) || defined(DEBUG_GESTURE_EVENTS))
                snx::Logger::log("Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n");
                snx::debug::cliLog("Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n");
#endif
                handler.touchDownTime = GetTime();

                break;
            }

            case GESTURE_DRAG:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered first GESTURE_DRAG\n");
                snx::debug::cliLog("Triggered first GESTURE_DRAG\n");
#endif
                //* Set modifier
                handler.modifier = true;

                Vector2 direction = Vector2MainDirection(GetGestureDragVector());
                if (direction == Vector2{0, -1})
                {
                    inputAction = InputActionId::ACT_UP;
                }

                else if (direction == Vector2{-1, 0})
                {
                    inputAction = InputActionId::ACT_LEFT;
                }

                else if (direction == Vector2{0, 1})
                {
                    inputAction = InputActionId::ACT_DOWN;
                }

                else if (direction == Vector2{1, 0})
                {
                    inputAction = InputActionId::ACT_RIGHT;
                }

                break;
            }

            case GESTURE_SWIPE_UP:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_UP\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_UP\n");
#endif
                inputAction = InputActionId::ACT_UP;

                break;
            }

            case GESTURE_SWIPE_LEFT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_LEFT\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_LEFT\n");
#endif
                inputAction = InputActionId::ACT_LEFT;

                break;
            }

            case GESTURE_SWIPE_DOWN:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_DOWN\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_DOWN\n");
#endif
                inputAction = InputActionId::ACT_DOWN;

                break;
            }

            case GESTURE_SWIPE_RIGHT:
            {
#if defined(DEBUG) && defined(DEBUG_GESTURES)
                snx::Logger::log("Triggered GESTURE_SWIPE_RIGHT\n");
                snx::debug::cliLog("Triggered GESTURE_SWIPE_RIGHT\n");
#endif
                inputAction = InputActionId::ACT_RIGHT;

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
        switch (handler.currentGesture)
        {
            default:
            case GESTURE_NONE:
            {
                inputAction = InputActionId::NONE;

                break;
            }

            case GESTURE_HOLD:
            {
                handler.touchHoldDuration = GetTime() - handler.touchDownTime;

                if ((handler.touchHoldDuration) > minHoldTime)
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
                handler.modifier = true;

                Vector2 direction = Vector2MainDirection(GetGestureDragVector());
                if (direction == Vector2{0, -1})
                {
                    inputAction = InputActionId::ACT_UP;
                }

                else if (direction == Vector2{-1, 0})
                {
                    inputAction = InputActionId::ACT_LEFT;
                }

                else if (direction == Vector2{0, 1})
                {
                    inputAction = InputActionId::ACT_DOWN;
                }

                else if (direction == Vector2{1, 0})
                {
                    inputAction = InputActionId::ACT_RIGHT;
                }

                break;
            }
        }
    }

    return inputAction;
}

