#include "InputHandler.h"
#define DEBUG_GESTURES

#include "Cursor.h"
#include "Directions.h"
#include "GameCamera.h"
#include "Hero.h"
#include "InputActionID.h"
#include "Logger.h"
#include "Map.h"
#include "Pathfinder.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <utility>

// private
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

    bindKey(KEY_SPACE, InputActionID::interact);

    bindModifierKey(KEY_LEFT_SHIFT, InputActionID::mod);

    bindMouseButton(MOUSE_BUTTON_RIGHT, InputActionID::cursorToggle);
    bindMouseButton(MOUSE_BUTTON_LEFT, InputActionID::moveToTarget);
}

bool InputHandler::takeInputMouse(bool isCursorActive)
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
    if (
        inputAction_ == InputActionID::none
        || (!(inputAction_ == InputActionID::cursorToggle)
            && !isCursorActive))
    {

        return false;
    }

    return true;
}

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
#ifdef TERMUX
    if ((modifier_ && !currentKey))
#else
    if ((modifier_ && !currentKey) || IsKeyPressedRepeat(lastKey))
#endif
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
    // GESTURE_DRAG is detected continuously
    if (currentGesture != GESTURE_DRAG)
    {
        lastGesture = currentGesture;
    }

    currentGesture = GetGestureDetected();

    // Check modifiers
    modifier_ = IsGestureDetected(GESTURE_DRAG);

    if (currentGesture != lastGesture)
    {
        // Store gesture string
        switch (currentGesture)
        {
        case GESTURE_SWIPE_UP:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered SWIPE_UP\n");
#endif
            inputAction_ = InputActionID::actUp;
            break;
        }

        case GESTURE_SWIPE_LEFT:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered SWIPE_LEFT\n");
#endif
            inputAction_ = InputActionID::actLeft;
            break;
        }

        case GESTURE_SWIPE_DOWN:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered SWIPE_DOWN\n");
#endif
            inputAction_ = InputActionID::actDown;
            break;
        }

        case GESTURE_SWIPE_RIGHT:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered SWIPE_RIGHT\n");
#endif
            inputAction_ = InputActionID::actRight;
            break;
        }

        case GESTURE_TAP:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered TAP\n");
#endif
            break;
        }

        case GESTURE_DOUBLETAP:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered DOUBLETAP\n");
#endif
            inputAction_ = InputActionID::interact;
            break;
        }

        case GESTURE_HOLD:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered HOLD\n");
#endif
            break;
        }

        case GESTURE_DRAG:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered DRAG\n");
#endif
            Vector2 direction = Vector2MainDirection(GetGestureDragVector());
            if (Vector2Equals(direction, {0, -1}))
            {
                inputAction_ = InputActionID::actUp;
            }
            else if (Vector2Equals(direction, {-1, 0}))
            {
                inputAction_ = InputActionID::actLeft;
            }
            else if (Vector2Equals(direction, {0, 1}))
            {
                inputAction_ = InputActionID::actDown;
            }
            else if (Vector2Equals(direction, {1, 0}))
            {
                inputAction_ = InputActionID::actRight;
            }
            break;
        }

        case GESTURE_PINCH_IN:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered PINCH_IN\n");
#endif
            break;
        }

        case GESTURE_PINCH_OUT:
        {
#ifdef DEBUG_GESTURES
            snx::Logger::log("Triggered PINCH_OUT\n");
#endif
            break;
        }

        case GESTURE_NONE:
        default:
        {
            inputAction_ = InputActionID::none;
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

// public
void InputHandler::takeInput(bool isCursorActive)
{
    if (takeInputKey())
    {
        return;
    }

    if (takeInputMouse(isCursorActive))
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
    Cursor& cursor,
    Map& map,
    GameCamera const& gameCamera)
{

    if (inputAction_ == InputActionID::none)
    {
        // Trigger input agnostic actions
        hero.movement().trigger();

        return;
    }

    switch (inputAction_)
    {
    case InputActionID::actUp:
    {
        hero.movement().trigger(
            Directions::V_UP);
    }
    break;

    case InputActionID::actLeft:
    {
        hero.movement().trigger(
            Directions::V_LEFT);
    }
    break;

    case InputActionID::actDown:
    {
        hero.movement().trigger(
            Directions::V_DOWN);
    }
    break;

    case InputActionID::actRight:
    {
        hero.movement().trigger(
            Directions::V_RIGHT);
    }
    break;

    case InputActionID::cursorToggle:
    {
        cursor.toggle();
    }
    break;

    case InputActionID::moveToTarget:
    {
        hero.movement().trigger(Pathfinder::findPath(
            map,
            hero.position().tilePosition(),
            cursor.position().tilePosition(),
            gameCamera));
    }
    break;

    case InputActionID::interact:
    {
        Vector2I heroTilePosition{hero.position().tilePosition()};

        if (!map.objects().events().contains(heroTilePosition))
        {
            snx::Logger::log("Nothing to do...");
            break;
        }

        snx::PublisherStatic::publish(map.objects().event(heroTilePosition));
    }
    break;

    default:
        break;
    }

    // Reset
    inputAction_ = InputActionID::none;
}
