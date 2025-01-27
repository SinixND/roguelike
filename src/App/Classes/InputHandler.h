#ifndef IG20250119163144
#define IG20250119163144

#include "InputId.h"
#include <raylib.h>
#include <unordered_map>

struct InputHandler
{
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

namespace InputModule
{
    InputId fromKeyboard(
        InputHandler* inputHandler,
        std::unordered_map<int, InputId> const& keyboardToInput,
        std::unordered_map<InputId, int> const& modifiers
    );

    InputId fromMouse(
        bool* isCursorActive,
        std::unordered_map<int, InputId> const& mouseToInput
    );

    InputId fromGesture( InputHandler* handler );
}

#endif
