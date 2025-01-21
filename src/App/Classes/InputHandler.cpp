#include "InputHandler.h"

#include "EventId.h"
#include "InputId.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>

//* Maximum hold time for tap event to trigger
double constexpr maxTapTime{ 0.3f };
//* Minimum hold time for hold event to trigger
double constexpr minHoldTime{ 0.3f };
//* Maximum time between taps for double tap event to trigger
double constexpr maxDoubleTapTime{ 0.3f };

namespace InputModule
{
    InputId fromKeyboard(
        InputHandler* inputHandler,
        std::unordered_map<int, InputId> const& keyboardToInput,
        std::unordered_map<InputId, int> const& modifiers
    )
    {
        //* Update key pressed
        //* Set lastKey only to valid inputs (associated with actions)
        inputHandler->lastKey = ( inputHandler->currentKey ) ? inputHandler->currentKey : inputHandler->lastKey;
        inputHandler->currentKey = GetKeyPressed();

        //* Check modifiers
        inputHandler->isModifierActive = IsKeyDown( modifiers.at( InputId::MOD ) );

        //* Repeat last key if no input but modifier down
        if ( ( inputHandler->isModifierActive
               && !inputHandler->currentKey )
#if !defined( TERMUX )
             || IsKeyPressedRepeat( inputHandler.lastKey )
#endif
        )
        {
            inputHandler->currentKey = inputHandler->lastKey;
        }

        if ( !keyboardToInput.contains( inputHandler->currentKey ) )
        {
            return InputId::NONE;
        }

        return keyboardToInput.at( inputHandler->currentKey );
    }

    InputId fromMouse(
        bool* isCursorActive,
        std::unordered_map<int, InputId> const& mouseToInput
    )
    {
        InputId inputId{ InputId::NONE };

        for ( int mouseButton : { MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT } )
        {
            if ( ( isCursorActive
                   || mouseToInput.at( mouseButton ) == InputId::TOGGLE_CURSOR )
                 && IsMouseButtonPressed( mouseButton ) )
            {
                inputId = mouseToInput.at( mouseButton );
            }
        }

        return inputId;
    }

    InputId fromGesture( InputHandler* inputHandler )
    {
        InputId inputId{ InputId::NONE };

        //* IMPORTANT NOTE:
        //* Implemented events TAP, DOUBLETAP and HOLD as raylib gesture registration was unreliable
        //* Therefore disabled any functionality of GESTURE_TAP and GESTURE_DOUBLE_TAP
        //* but left in game logging so it gets noticed when it was fixed

        //* Update gestures
        inputHandler->lastGesture = inputHandler->currentGesture;
        inputHandler->currentGesture = GetGestureDetected();

        //* Detect gesture change
        if ( inputHandler->currentGesture != inputHandler->lastGesture )
        {
            switch ( inputHandler->currentGesture )
            {
                default:
                case GESTURE_NONE:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_NONE\n" );
                    snx::debug::cliLog( "Triggered GESTURE_NONE\n" );
#endif
#if defined( DEBUG ) && defined( DEBUG_GESTURE_EVENTS )
                    snx::Logger::log( "Triggered TOUCH UP EVENT\n" );
                    snx::debug::cliLog( "Triggered TOUCH UP EVENT\n" );
#endif
                    inputHandler->touchUpTime = GetTime();

                    //* Reset hold duration
                    inputHandler->touchHoldDuration = 0;

                    //* Check for Tap events
                    if ( inputHandler->lastGesture == GESTURE_HOLD
                         && ( inputHandler->touchUpTime - inputHandler->touchDownTime ) < maxTapTime )
                    {
                        //* Check for double tap
                        if ( ( inputHandler->touchUpTime - inputHandler->lastTap ) < maxDoubleTapTime )
                        {
#if defined( DEBUG ) && defined( DEBUG_GESTURE_EVENTS )
                            snx::Logger::log( "Triggered DOUBLE TAP EVENT\n" );
                            snx::debug::cliLog( "Triggered DOUBLE TAP EVENT\n" );
#endif
                            inputId = InputId::ACT_IN_PLACE;
                        }
                        else
                        {
#if defined( DEBUG ) && defined( DEBUG_GESTURE_EVENTS )
                            snx::Logger::log( "Triggered TAP EVENT\n" );
                            snx::debug::cliLog( "Triggered TAP EVENT\n" );
#endif
                        }

                        inputHandler->lastTap = inputHandler->touchUpTime;

                        break;
                    }

                    inputId = InputId::NONE;

                    break;
                }

                case GESTURE_TAP:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_TAP\n" );
#endif
                    // touchDownTime = GetTime();

                    break;
                }

                case GESTURE_DOUBLETAP:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_DOUBLETAP\n" );
#endif
                    // inputInput = InputId::actInPlace;

                    break;
                }

                case GESTURE_HOLD:
                {
#if defined( DEBUG ) && ( defined( DEBUG_GESTURES ) || defined( DEBUG_GESTURE_EVENTS ) )
                    snx::Logger::log( "Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n" );
                    snx::debug::cliLog( "Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n" );
#endif
                    inputHandler->touchDownTime = GetTime();

                    break;
                }

                case GESTURE_DRAG:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered first GESTURE_DRAG\n" );
                    snx::debug::cliLog( "Triggered first GESTURE_DRAG\n" );
#endif
                    //* Set modifier
                    inputHandler->isModifierActive = true;

                    Vector2 direction = Vector2MainDirection( GetGestureDragVector() );
                    if ( direction == Vector2{ 0, -1 } )
                    {
                        inputId = InputId::ACT_UP;
                    }

                    else if ( direction == Vector2{ -1, 0 } )
                    {
                        inputId = InputId::ACT_LEFT;
                    }

                    else if ( direction == Vector2{ 0, 1 } )
                    {
                        inputId = InputId::ACT_DOWN;
                    }

                    else if ( direction == Vector2{ 1, 0 } )
                    {
                        inputId = InputId::ACT_RIGHT;
                    }

                    break;
                }

                case GESTURE_SWIPE_UP:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_SWIPE_UP\n" );
                    snx::debug::cliLog( "Triggered GESTURE_SWIPE_UP\n" );
#endif
                    inputId = InputId::ACT_UP;

                    break;
                }

                case GESTURE_SWIPE_LEFT:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_SWIPE_LEFT\n" );
                    snx::debug::cliLog( "Triggered GESTURE_SWIPE_LEFT\n" );
#endif
                    inputId = InputId::ACT_LEFT;

                    break;
                }

                case GESTURE_SWIPE_DOWN:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_SWIPE_DOWN\n" );
                    snx::debug::cliLog( "Triggered GESTURE_SWIPE_DOWN\n" );
#endif
                    inputId = InputId::ACT_DOWN;

                    break;
                }

                case GESTURE_SWIPE_RIGHT:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_SWIPE_RIGHT\n" );
                    snx::debug::cliLog( "Triggered GESTURE_SWIPE_RIGHT\n" );
#endif
                    inputId = InputId::ACT_RIGHT;

                    break;
                }

                case GESTURE_PINCH_IN:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_PINCH_IN\n" );
                    snx::debug::cliLog( "Triggered GESTURE_PINCH_IN\n" );
#endif

                    break;
                }

                case GESTURE_PINCH_OUT:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_PINCH_OUT\n" );
                    snx::debug::cliLog( "Triggered GESTURE_PINCH_OUT\n" );
#endif

                    break;
                }
            }
        }

        //* Detect continuous gestures
        //* if (currentGesture_ == lastGesture_)
        else
        {
            switch ( inputHandler->currentGesture )
            {
                default:
                case GESTURE_NONE:
                {
                    inputId = InputId::NONE;

                    break;
                }

                case GESTURE_HOLD:
                {
                    inputHandler->touchHoldDuration = GetTime() - inputHandler->touchDownTime;

                    if ( ( inputHandler->touchHoldDuration ) > minHoldTime )
                    {
#if defined( DEBUG ) && defined( DEBUG_GESTURE_EVENTS )
                        snx::Logger::log( "Triggered HOLD EVENT\n" );
                        snx::debug::cliLog( "Triggered HOLD EVENT\n" );
#endif
                        //* Get/Set info panel reference to tile/object/enemy at current position
                        snx::PublisherStatic::publish( EventId::CURSOR_POSITION_CHANGED );
                    }

                    break;
                }

                case GESTURE_DRAG:
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                    snx::Logger::log( "Triggered GESTURE_DRAG\n" );
                    snx::debug::cliLog( "Triggered GESTURE_DRAG\n" );
#endif
                    //* Set modifier
                    inputHandler->isModifierActive = true;

                    Vector2 direction = Vector2MainDirection( GetGestureDragVector() );
                    if ( direction == Vector2{ 0, -1 } )
                    {
                        inputId = InputId::ACT_UP;
                    }

                    else if ( direction == Vector2{ -1, 0 } )
                    {
                        inputId = InputId::ACT_LEFT;
                    }

                    else if ( direction == Vector2{ 0, 1 } )
                    {
                        inputId = InputId::ACT_DOWN;
                    }

                    else if ( direction == Vector2{ 1, 0 } )
                    {
                        inputId = InputId::ACT_RIGHT;
                    }

                    break;
                }
            }
        }

        return inputId;
    }
}
