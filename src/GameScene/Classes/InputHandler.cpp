#include "InputHandler.h"

#include "Debugger.h"
#include "Directions.h"
#include "Hero.h"
#include "InputActionID.h"
#include "Logger.h"
#include <raylib.h>
#include <string>
#include <utility>

// private
void InputHandler::bindKey(int key, InputActionID action)
{
    keyToInputActionID_.insert(std::make_pair(key, action));
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

    bindModifierKey(KEY_LEFT_SHIFT, InputActionID::mod);
};

bool InputHandler::takeInputKey()
{
    static int currentKey = KEY_NULL;
    static int lastKey = KEY_NULL;

    // Update key pressed
    // Set lastKey only to valid inputs (associated with actions)
    lastKey = (currentKey) ? currentKey : lastKey;
    currentKey = GetKeyPressed();

    // Check modifiers
    modifier_ = IsKeyDown(inputActionIDToModifierKey_[InputActionID::mod]);

    // Repeat last key if no input but modifier down
    if (modifier_ && !currentKey)
    {
        currentKey = lastKey;
    }

    inputAction_ = keyToInputActionID_[currentKey];

    if (inputAction_ == InputActionID::none)
    {
        return false;
    }

    return true;
}

bool InputHandler::takeInputGesture()
{
    static int currentGesture = GESTURE_NONE;
    static int lastGesture = GESTURE_NONE;

    // Update gesture
    // Set lastGesture only to valid inputs (associated with actions)
    lastGesture = (
            currentGesture == GESTURE_SWIPE_UP
            || currentGesture == GESTURE_SWIPE_LEFT
            || currentGesture == GESTURE_SWIPE_DOWN
            || currentGesture == GESTURE_SWIPE_RIGHT
            ) ? currentGesture : lastGesture;

    currentGesture = GetGestureDetected();

    // Check modifiers
    modifier_ = IsGestureDetected(GESTURE_HOLD);

    // Repeat last gesture if no input but modifier down
    if (modifier_ && !(
            currentGesture == GESTURE_SWIPE_UP
            || currentGesture == GESTURE_SWIPE_LEFT
            || currentGesture == GESTURE_SWIPE_DOWN
            || currentGesture == GESTURE_SWIPE_RIGHT
        ))
    {
        currentGesture = lastGesture;
    }

    if (currentGesture != lastGesture)
    {
        // Store gesture string
        switch (currentGesture)
        {
        case GESTURE_SWIPE_UP:
            inputAction_ = InputActionID::actUp;
            break;

        case GESTURE_SWIPE_LEFT:
            inputAction_ = InputActionID::actLeft;
            break;

        case GESTURE_SWIPE_DOWN:
            inputAction_ = InputActionID::actDown;
            break;

        case GESTURE_SWIPE_RIGHT:
            inputAction_ = InputActionID::actRight;
            break;

        case GESTURE_TAP:
            break;

        case GESTURE_DOUBLETAP:
            break;

        case GESTURE_HOLD:
            break;

        case GESTURE_DRAG:
            break;

        case GESTURE_PINCH_IN:
            break;

        case GESTURE_PINCH_OUT:
            break;

        case GESTURE_NONE:
        default:
            inputAction_ = InputActionID::none;
            break;
        }
    }

    if (inputAction_ == InputActionID::none)
    {
        return false;
    }

    return true;
}

// public
void InputHandler::takeInput()
{
    if (takeInputKey())
    {
        return;
    }

    takeInputGesture();
}

void InputHandler::triggerAction(Hero& hero)
{
    if (inputAction_ == InputActionID::none)
    {
        return;
    }

    switch (inputAction_)
    {
    case InputActionID::actUp:
    {
        hero.movement().trigger(
            Directions::V_UP,
            modifier_);
    }
    break;

    case InputActionID::actLeft:
    {
        hero.movement().trigger(
            Directions::V_LEFT,
            modifier_);
    }
    break;

    case InputActionID::actDown:
    {
        hero.movement().trigger(
            Directions::V_DOWN,
            modifier_);
    }
    break;

    case InputActionID::actRight:
    {
        hero.movement().trigger(
            Directions::V_RIGHT,
            modifier_);
    }
    break;

    default:
        break;
    }

    // Reset
    inputAction_ = InputActionID::none;
}

//     void InputHandler::simulateInput(int key)
// {
//     
// }
