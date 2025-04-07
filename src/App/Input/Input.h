#ifndef IG20250119163144
#define IG20250119163144

#include "Cursor.h"
#include "InputId.h"
#include "InputMappings.h"
#include <raylib.h>

class Input
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

public:
    InputMappings mappings{};

    Cursor cursor{};

    InputId currentInputId{ InputId::NONE };

public:
    InputId fromKeyboard();
    InputId fromMouse();
    InputId fromGesture();
};

#endif
