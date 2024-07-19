#include "InputHandler.h"

#include "Directions.h"
#include "Hero.h"
#include "HeroActionID.h"
#include <raylib.h>

void InputHandler::bind(int key, HeroActionID action)
{
    keyToHeroActionID_.insert(std::make_pair(key, action));
}

void InputHandler::setDefaultInputMappings()
{
    bind(KEY_NULL, HeroActionID::none);

    bind(KEY_W, HeroActionID::moveUp);
    bind(KEY_K, HeroActionID::moveUp);
    bind(KEY_UP, HeroActionID::moveUp);

    bind(KEY_A, HeroActionID::moveLeft);
    bind(KEY_K, HeroActionID::moveLeft);
    bind(KEY_LEFT, HeroActionID::moveLeft);

    bind(KEY_S, HeroActionID::moveDown);
    bind(KEY_J, HeroActionID::moveDown);
    bind(KEY_DOWN, HeroActionID::moveDown);

    bind(KEY_D, HeroActionID::moveRight);
    bind(KEY_L, HeroActionID::moveRight);
    bind(KEY_RIGHT, HeroActionID::moveRight);
};


void InputHandler::handleInput()
{
    if (handleKey()) return;
    if (handleGesture()) return;
}

bool InputHandler::handleKey()
{
    heroAction_ = keyToHeroActionID_[GetKeyPressed()];

    if (heroAction_ == HeroActionID::none)
    {
        return false;
    }

    return true;
}

bool InputHandler::handleGesture()
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
        //case GESTURE_TAP:
            //break;

        //case GESTURE_DOUBLETAP:
            //break;

        //case GESTURE_HOLD:
            //break;

        //case GESTURE_DRAG:
            //break;

        case GESTURE_SWIPE_UP:
            heroAction_ = HeroActionID::moveUp;
            break;

        case GESTURE_SWIPE_LEFT:
            heroAction_ = HeroActionID::moveLeft;
            break;

        case GESTURE_SWIPE_DOWN:
            heroAction_ = HeroActionID::moveDown;
            break;

        case GESTURE_SWIPE_RIGHT:
            heroAction_ = HeroActionID::moveRight;
            break;

        //case GESTURE_PINCH_IN:
            //break;

        //case GESTURE_PINCH_OUT:
            //break;

        case GESTURE_NONE:
        default:
            heroAction_ = HeroActionID::none;
            break;

        }
    }

    if (heroAction_ == HeroActionID::none)
    {
        return false;
    }

    return true;
}

void InputHandler::update(Hero* hero)
{
    if (heroAction_ == HeroActionID::none)
    {
        return;
    }

    switch (heroAction_)
    {
    case HeroActionID::moveUp:
    {
        // Default action (without shift)
        if (!IsKeyDown(KEY_LEFT_SHIFT))
        {
            hero->triggerMovement(
                Directions::V_UP,
                &hero->movement(),
                &hero->energy());
        }

        // Alternate action if shift is held
    }
    break;

    case HeroActionID::moveLeft:
    {
        // Default action (without shift)
        if (!IsKeyDown(KEY_LEFT_SHIFT))
        {
            hero->triggerMovement(
                Directions::V_LEFT,
                &hero->movement(),
                &hero->energy());
        }

        // Alternate action if shift is held
    }
    break;

    case HeroActionID::moveDown:
    {
        // Default action (without shift)
        if (!IsKeyDown(KEY_LEFT_SHIFT))
        {
            hero->triggerMovement(
                Directions::V_DOWN,
                &hero->movement(),
                &hero->energy());
        }

        // Alternate action if shift is held
    }
    break;

    case HeroActionID::moveRight:
    {
        // Default action (without shift)
        if (!IsKeyDown(KEY_LEFT_SHIFT))
        {
            hero->triggerMovement(
                Directions::V_RIGHT,
                &hero->movement(),
                &hero->energy());
        }

        // Alternate action if shift is held
    }
    break;

    default:
        break;
    }
}
