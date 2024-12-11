#ifndef IG20241027225100
#define IG20241027225100

#include "InputActionId.h"
#include <raylib.h>
#include <unordered_map>

class UserInput
{
    bool modifier_{};

    std::unordered_map<int, InputActionId> keyToInputActionId_{};
    std::unordered_map<int, InputActionId> mouseButtonToInputActionId_{};
    std::unordered_map<InputActionId, int> inputActionIdToModifierKey_{};

    int currentKey_ = KEY_NULL;
    int lastKey_ = KEY_NULL;

    int currentGesture_ = GESTURE_NONE;
    int lastGesture_ = GESTURE_NONE;

    double touchDownTime_{0};
    double touchUpTime_{0.01};
    double touchHoldDuration_{};
    double lastTap_{};

public:
    InputActionId inputAction{InputActionId::NONE};

public:
    void bindKey(int key, InputActionId action);
    void bindMouseButton(int key, InputActionId action);
    void bindModifierKey(int key, InputActionId action);

    //* Returns true if input received
    bool registerMouse(bool isCursorActive);

    //* Returns true if input received
    bool registerKeyboard();

    //* Returns true if input received
    bool registerGesture();

    void resetInputAction();
};

#endif
