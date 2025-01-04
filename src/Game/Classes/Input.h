#ifndef IG20241027225100
#define IG20241027225100

#include "InputActionId.h"
#include "InputMappings.h"
#include <raylib.h>

struct Input
{
    int currentKey = KEY_NULL;
    int lastKey = KEY_NULL;

    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    double touchDownTime{0};
    double touchUpTime{0.01};
    double touchHoldDuration{};
    double lastTap{};

    bool modifier{};
};

namespace InputModule
{
    InputActionId checkKeyboard(
        Input& input,
        InputMappings const& mappings);

    InputActionId checkMouse(
        InputMappings const& mappings,
        bool const isCursorActive);

    InputActionId checkGesture(Input& handler);
}

#endif