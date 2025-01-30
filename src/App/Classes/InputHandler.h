#ifndef IG20250119163144
#define IG20250119163144

#include "InputId.h"
#include "InputMappings.h"
#include <raylib.h>

class InputHandler
{
public:
    InputMappings mappings;

public:
    InputId fromKeyboard();
    InputId fromMouse();
    InputId fromGesture();

private:
    int currentKey{ KEY_NULL };
    int lastKey{ KEY_NULL };

    int currentGesture = { GESTURE_NONE };
    int lastGesture = { GESTURE_NONE };

    double touchDownTime{ 0 };
    double touchUpTime{ 0.01 };
    double touchHoldDuration{};
    double lastTap{};

    bool isModifierActive{};
    bool isCursorActive{};
};

#endif
