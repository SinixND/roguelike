#include "InputHandler.h"

#include "Directions.h"
#include "Hero.h"
#include "InputActionID.h"
#include <raylib.h>
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

    bindModifierKey(KEY_LEFT_SHIFT, InputActionID::modShift);
};

bool InputHandler::takeInputKey()
{
    // static int key{GetKeyPressed()};
    //
    // if (IsKeyUp(key))
    // {
    //     key = KEY_NULL;
    // }
    //
    // Get action mapped to key. If no key pressed, set null-action
    // if (IsKeyDown(key))
    // {
    //     inputAction_ = keyToInputActionID_[key];
    // }
    inputAction_ = keyToInputActionID_[GetKeyPressed()];

    // Check modifiers
    modifierShift_ = IsKeyDown(inputActionIDToModifierKey_[InputActionID::modShift]);

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
    lastGesture = currentGesture;
    currentGesture = GetGestureDetected();

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

void InputHandler::triggerInput(Hero& hero)
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
            modifierShift_);
    }
    break;

    case InputActionID::actLeft:
    {
        hero.movement().trigger(
            Directions::V_LEFT,
            modifierShift_);
    }
    break;

    case InputActionID::actDown:
    {
        hero.movement().trigger(
            Directions::V_DOWN,
            modifierShift_);
    }
    break;

    case InputActionID::actRight:
    {
        hero.movement().trigger(
            Directions::V_RIGHT,
            modifierShift_);
    }
    break;

    default:
        break;
    }

    // Reset
    inputAction_ = InputActionID::none;
}
