#include "InputHandler.h"

#include "Directions.h"
#include "Hero.h"
#include "InputProperties.h"
#include "raylibEx.h"
#include <raylib.h>

void InputHandler::check()
{
    // Store last key pressed
    int keyPressed{GetKeyPressed()};

    if (keyPressed)
    {
        keyCached = keyPressed;
    }
}

void InputHandler::update(Hero* hero)
{
    if (!IsKeyPressedRepeat(keyCached, INPUT_DELAY, INPUT_TICK))
    {
        return;
    }

    switch (keyCached)
    {
    case KEY_W:
    case KEY_UP:
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

    case KEY_A:
    case KEY_LEFT:
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

    case KEY_S:
    case KEY_DOWN:
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

    case KEY_D:
    case KEY_RIGHT:
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