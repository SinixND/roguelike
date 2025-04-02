#include "InputHandler.h"

// #define DEBUG_GESTURES

#include "EventDispatcher.h"
#include "EventId.h"
#include "InputData.h"
#include "InputId.h"
#include "raylibEx.h"
#include <raylib.h>

#if defined( DEBUG ) && defined( DEBUG_GESTURES )
#include "Debugger.h"
#include "Logger.h"
#endif

InputId InputHandler::fromKeyboard()
{
    //* Update key pressed
    //* Set lastKey only to valid inputs (associated with actions)
    lastKey_ = ( currentKey_ ) ? currentKey_ : lastKey_;
    currentKey_ = GetKeyPressed();

    //* Check modifiers
    isModifierActive_ = IsKeyDown( mappings.modifierToKeyboard.at( InputId::MOD ) );

    //* Repeat last key if
    //* - modifier down
    //* - no input
    //* - last key is repeatable (directional input)
    if ( ( isModifierActive_
           && !currentKey_
           && ( mappings.keyboardToInput.at( lastKey_ ) == InputId::ACT_LEFT
                || mappings.keyboardToInput.at( lastKey_ ) == InputId::ACT_DOWN
                || mappings.keyboardToInput.at( lastKey_ ) == InputId::ACT_UP
                || mappings.keyboardToInput.at( lastKey_ ) == InputId::ACT_RIGHT ) )
#if !defined( TERMUX )
         || IsKeyPressedRepeat( lastKey_ )
#endif
    )
    {
        currentKey_ = lastKey_;
    }

    if ( !mappings.keyboardToInput.contains( currentKey_ ) )
    {
        return InputId::NONE;
    }

    return mappings.keyboardToInput.at( currentKey_ );
}

InputId InputHandler::fromMouse()
{
    InputId inputId{ InputId::NONE };

    for ( int mouseButton : {
              MOUSE_BUTTON_LEFT,
              MOUSE_BUTTON_RIGHT
          } )
    {
        if ( ( isCursorActive_
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

    // if ( isCursorActive_ )
    // {
    //     return inputId;
    // }

    //* IMPORTANT NOTE:
    //* Implemented events TAP, DOUBLETAP and HOLD as raylib gesture registration was unreliable
    //* Therefore disabled any functionality of GESTURE_TAP and GESTURE_DOUBLE_TAP
    //* but left in game logging so it gets noticed when it was fixed

    //* Update gestures
    lastGesture = currentGesture_;
    currentGesture_ = GetGestureDetected();

    //* Detect gesture change
    if ( currentGesture_ != lastGesture )
    {
        switch ( currentGesture_ )
        {
            default:
            case GESTURE_NONE:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered GESTURE_NONE\n" );
                snx::Debugger::cliLog( "Triggered GESTURE_NONE\n" );
#endif
#if defined( DEBUG ) && defined( DEBUG_GESTURE_EVENTS )
                snx::Logger::log( "Triggered TOUCH UP EVENT\n" );
                snx::Debugger::cliLog( "Triggered TOUCH UP EVENT\n" );
#endif
                touchUpTime_ = GetTime();

                //* Reset hold duration
                touchHoldDuration_ = 0;

                //* Check for Tap events
                if ( lastGesture == GESTURE_HOLD
                     && ( touchUpTime_ - touchDownTime_ ) < InputData::MAX_TAP_TIME )
                {
                    //* Check for double tap
                    if ( ( touchUpTime_ - lastTap_ ) < InputData::MAX_DOUBLE_TAP_TIME )
                    {
#if defined( DEBUG ) && defined( DEBUG_GESTURE_EVENTS )
                        snx::Logger::log( "Triggered DOUBLE TAP EVENT\n" );
                        snx::Debugger::cliLog( "Triggered DOUBLE TAP EVENT\n" );
#endif
                        inputId = InputId::ACT_IN_PLACE;
                    }
                    else if ( isCursorActive_
                              && touchHoldDuration_ < InputData::MAX_TAP_TIME )
                    {
#if defined( DEBUG ) && defined( DEBUG_GESTURE_EVENTS )
                        snx::Logger::log( "Triggered TAP EVENT\n" );
                        snx::Debugger::cliLog( "Triggered TAP EVENT\n" );
#endif
                        inputId = InputId::ACT_TO_TARGET;
                    }

                    lastTap_ = touchUpTime_;

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
                snx::Debugger::cliLog( "Triggered first GESTURE_HOLD (TOUCH DOWN EVENT)\n" );
#endif
                touchDownTime_ = GetTime();

                break;
            }

            case GESTURE_DRAG:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered first GESTURE_DRAG\n" );
                snx::Debugger::cliLog( "Triggered first GESTURE_DRAG\n" );
#endif
                if ( isCursorActive_ )
                {
                    break;
                }

                //* Set modifier
                isModifierActive_ = true;

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
                snx::Debugger::cliLog( "Triggered GESTURE_SWIPE_UP\n" );
#endif
                if ( isCursorActive_ )
                {
                    break;
                }

                inputId = InputId::ACT_UP;

                break;
            }

            case GESTURE_SWIPE_LEFT:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered GESTURE_SWIPE_LEFT\n" );
                snx::Debugger::cliLog( "Triggered GESTURE_SWIPE_LEFT\n" );
#endif
                if ( isCursorActive_ )
                {
                    break;
                }

                inputId = InputId::ACT_LEFT;

                break;
            }

            case GESTURE_SWIPE_DOWN:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered GESTURE_SWIPE_DOWN\n" );
                snx::Debugger::cliLog( "Triggered GESTURE_SWIPE_DOWN\n" );
#endif
                if ( isCursorActive_ )
                {
                    break;
                }

                inputId = InputId::ACT_DOWN;

                break;
            }

            case GESTURE_SWIPE_RIGHT:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered GESTURE_SWIPE_RIGHT\n" );
                snx::Debugger::cliLog( "Triggered GESTURE_SWIPE_RIGHT\n" );
#endif
                if ( isCursorActive_ )
                {
                    break;
                }

                inputId = InputId::ACT_RIGHT;

                break;
            }

            case GESTURE_PINCH_IN:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered GESTURE_PINCH_IN\n" );
                snx::Debugger::cliLog( "Triggered GESTURE_PINCH_IN\n" );
#endif

                break;
            }

            case GESTURE_PINCH_OUT:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered GESTURE_PINCH_OUT\n" );
                snx::Debugger::cliLog( "Triggered GESTURE_PINCH_OUT\n" );
#endif

                break;
            }
        }
    }

    //* Detect continuous gestures
    //* if (currentGesture_ == lastGesture_)
    else
    {
        switch ( currentGesture_ )
        {
            default:
            case GESTURE_NONE:
            {
                inputId = InputId::NONE;

                break;
            }

            case GESTURE_HOLD:
            {
                touchHoldDuration_ = GetTime() - touchDownTime_;

                if ( ( touchHoldDuration_ ) > InputData::MIN_HOLD_TIME )
                {
#if defined( DEBUG ) && defined( DEBUG_GESTURE_EVENTS )
                    snx::Logger::log( "Triggered HOLD EVENT\n" );
                    snx::Debugger::cliLog( "Triggered HOLD EVENT\n" );
#endif
                    //* Get/Set info panel reference to tile/object/enemy at current position
                    snx::EventDispatcher::notify( EventId::CURSOR_POSITION_CHANGED );
                }

                break;
            }

            case GESTURE_DRAG:
            {
#if defined( DEBUG ) && defined( DEBUG_GESTURES )
                snx::Logger::log( "Triggered GESTURE_DRAG\n" );
                snx::Debugger::cliLog( "Triggered GESTURE_DRAG\n" );
#endif
                if ( isCursorActive_ )
                {
                    break;
                }

                //* Set modifier
                isModifierActive_ = true;

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
    isCursorActive_ = !isCursorActive_;
}
