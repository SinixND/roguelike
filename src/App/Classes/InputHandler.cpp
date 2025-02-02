#include "InputHandler.h"

#define DEBUG_GESTURES

#include "EventId.h"
#include "InputId.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>

#if defined( DEBUG )
#include "Debugger.h"
#include "Logger.h"
#endif

//* Maximum hold time for tap event to trigger
double constexpr maxTapTime{ 0.3f };
//* Minimum hold time for hold event to trigger
double constexpr minHoldTime{ 0.3f };
//* Maximum time between taps for double tap event to trigger
double constexpr maxDoubleTapTime{ 0.3f };

InputId InputHandler::fromKeyboard()
{
    //* Update key pressed
    //* Set lastKey only to valid inputs (associated with actions)
    lastKey = ( currentKey ) ? currentKey : lastKey;
    currentKey = GetKeyPressed();

    //* Check modifiers
    isModifierActive = IsKeyDown( mappings.modifiers.at( InputId::MOD ) );

    //* Repeat last key if no input but modifier down
    if ( ( isModifierActive
           && !currentKey )
#if !defined( TERMUX )
         || IsKeyPressedRepeat( lastKey )
#endif
    )
    {
        currentKey = lastKey;
    }

    if ( !mappings.keyboardToInput.contains( currentKey ) )
    {
        return InputId::NONE;
    }

    return mappings.keyboardToInput.at( currentKey );
}

InputId InputHandler::fromMouse()
{
    InputId inputId{ InputId::NONE };

    for ( int mouseButton : { MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT } )
    {
        if ( ( isCursorActive
               || mappings.mouseToInput.at( mouseButton ) == InputId::TOGGLE_CURSOR )
             && IsMouseButtonPressed( mouseButton ) )
        {
            inputId = mappings.mouseToInput.at( mouseButton );
        }
    }

    return inputId;
}

InputId InputHandler::fromGesture()
{
    InputId inputId{ InputId::NONE };

    if ( isCursorActive )
    {
        return inputId;
    }

    //* IMPORTANT NOTE:
    //* Implemented events TAP, DOUBLETAP and HOLD as raylib gesture registration was unreliable
    //* Therefore disabled any functionality of GESTURE_TAP and GESTURE_DOUBLE_TAP
    //* but left in game logging so it gets noticed when it was fixed

    //* Update gestures
    lastGesture = currentGesture;
    currentGesture = GetGestureDetected();

    //* Detect gesture change
    if ( currentGesture != lastGesture )
    {
        switch ( currentGesture )
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
                touchUpTime = GetTime();

                //* Reset hold duration
                touchHoldDuration = 0;

                //* Check for Tap events
                if ( lastGesture == GESTURE_HOLD
                     && ( touchUpTime - touchDownTime ) < maxTapTime )
                {
                    //* Check for double tap
                    if ( ( touchUpTime - lastTap ) < maxDoubleTapTime )
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

                    lastTap = touchUpTime;

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
                touchDownTime = GetTime();

                break;
            }

            case GESTURE_DRAG:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered first GESTURE_DRAG\n" );
                snx::debug::cliLog( "Triggered first GESTURE_DRAG\n" );
#endif
                //* Set modifier
                isModifierActive = true;

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
        switch ( currentGesture )
        {
            default:
            case GESTURE_NONE:
            {
                inputId = InputId::NONE;

                break;
            }

            case GESTURE_HOLD:
            {
                touchHoldDuration = GetTime() - touchDownTime;

                if ( ( touchHoldDuration ) > minHoldTime )
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
                isModifierActive = true;

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

void InputHandler::toggleCursorState()
{
    isCursorActive = !isCursorActive;
}
