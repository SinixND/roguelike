#include "InputHandler.h"

#include "Directions.h"
#include "Hero.h"
#include "InputActionID.h"
#include <raylib.h>
#include <utility>

// private
void InputHandler::bind(int key, InputActionID action)
{
    keyToInputActionID_.insert(std::make_pair(key, action));
}

void InputHandler::setDefaultInputMappings()
{
    bind(KEY_NULL, InputActionID::none);

    bind(KEY_W, InputActionID::actUp);
    bind(KEY_K, InputActionID::actUp);
    bind(KEY_UP, InputActionID::actUp);

    bind(KEY_A, InputActionID::actLeft);
    bind(KEY_K, InputActionID::actLeft);
    bind(KEY_LEFT, InputActionID::actLeft);

    bind(KEY_S, InputActionID::actDown);
    bind(KEY_J, InputActionID::actDown);
    bind(KEY_DOWN, InputActionID::actDown);

    bind(KEY_D, InputActionID::actRight);
    bind(KEY_L, InputActionID::actRight);
    bind(KEY_RIGHT, InputActionID::actRight);
};

bool InputHandler::takeInputKey()
{
    // Get action mapped to key. If no key pressed, set null-action
    inputAction_ = keyToInputActionID_[GetKeyPressed()];

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

    bool continuousMovement = false;

    switch (inputAction_)
    {
    case InputActionID::actUp:
    {
        // if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
        // {
        //     continuousMovement = true;
        // }

        hero.movement().trigger(
            Directions::V_UP,
            continuousMovement);
    }
    break;

    case InputActionID::actLeft:
    {
        // if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
        // {
        //     continuousMovement = true;
        // }

        hero.movement().trigger(
            Directions::V_LEFT,
            continuousMovement);
    }
    break;

    case InputActionID::actDown:
    {
        // if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
        // {
        //     continuousMovement = true;
        // }

        hero.movement().trigger(
            Directions::V_DOWN,
            continuousMovement);
    }
    break;

    case InputActionID::actRight:
    {
        // if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
        // {
        //     continuousMovement = true;
        // }

        hero.movement().trigger(
            Directions::V_RIGHT,
            continuousMovement);
    }
    break;

    default:
        break;
    }

    // Reset
    inputAction_ = InputActionID::none;
}
