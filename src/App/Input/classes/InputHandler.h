#ifndef IG20250119163144
#define IG20250119163144

#include "InputId.h"
#include "InputMappings.h"
#include <raylib.h>

class InputHandler
{
    int currentKey_{ KEY_NULL };
    int lastKey_{ KEY_NULL };

    int currentGesture_{ GESTURE_NONE };
    int lastGesture{ GESTURE_NONE };

    float touchDownTime_{ 0 };
    float touchUpTime_{ 0.01 };
    float touchHoldDuration_{ 0 };
    float lastTap_{ 0 };

    bool isModifierActive_{ false };
    bool isCursorActive_{ false };

public:
    InputMappings mappings;

    InputId currentInputId{ InputId::NONE };

public:
    InputId fromKeyboard();
    InputId fromMouse();
    InputId fromGesture();

    void toggleCursorState();
};

#endif
