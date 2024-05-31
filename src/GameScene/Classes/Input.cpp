#include "Input.h"
#include "Hero.h"
#include "raylibEx.h"
#include <raylib.h>

void Input::init(Hero* hero)
{
    hero_ = hero;
}

// Time in seconds unit input repeats
float constexpr INPUT_DELAY{0.25f};

// time in seconds between input repetition
float constexpr INPUT_TICK{0.1f};

void Input::check()
{
    auto key{GetKeyPressed()};

    if (!IsKeyPressedRepeat(key, INPUT_DELAY, INPUT_TICK))
    {
        return;
    }

    switch (key)
    {
    case KEY_W:
    case KEY_UP:
    {
        // Default action (without shift)
        if (!IsKeyDown(KEY_LEFT_SHIFT))
        {
        }

        // Alternate action if shift is held
    }
    break;

    case KEY_A:
    case KEY_LEFT:
    {
    }
    break;

    case KEY_S:
    case KEY_DOWN:
    {
    }
    break;

    case KEY_D:
    case KEY_RIGHT:
    {
    }
    break;

    default:
        break;
    }
}