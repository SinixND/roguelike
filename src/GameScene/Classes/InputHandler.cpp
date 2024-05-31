#include "InputHandler.h"

#include "Directions.h"
#include "Hero.h"
#include "InputProperties.h"
#include "raylibEx.h"
#include <raylib.h>

void InputHandler::check(Hero* hero)
{
    // Store last key pressed
    static int keyCached{};
    int keyPressed{GetKeyPressed()};

    if (keyPressed)
    {
        keyCached = keyPressed;
    }

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
            hero->movment().trigger(Directions::V_UP);
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
            hero->movment().trigger(Directions::V_LEFT);
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
            hero->movment().trigger(Directions::V_DOWN);
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
            hero->movment().trigger(Directions::V_RIGHT);
        }

        // Alternate action if shift is held
    }
    break;

    default:
        break;
    }
}