#ifndef IG20241027225100
#define IG20241027225100

#include "InputActionID.h"
#include <raylib.h>
#include <unordered_map>

class UserInputComponent
{
    InputActionID inputAction_{InputActionID::NONE};

    bool modifier_{};

    std::unordered_map<int, InputActionID> keyToInputActionID_{};
    std::unordered_map<int, InputActionID> mouseButtonToInputActionID_{};
    std::unordered_map<InputActionID, int> inputActionIDToModifierKey_{};

    int currentKey_ = KEY_NULL;
    int lastKey_ = KEY_NULL;

    int currentGesture_ = GESTURE_NONE;
    int lastGesture_ = GESTURE_NONE;

    double touchDownTime_{0};
    double touchUpTime_{0.01};
    double touchHoldDuration_{};
    double lastTap_{};

public:
    void bindKey(int key, InputActionID action);
    void bindMouseButton(int key, InputActionID action);
    void bindModifierKey(int key, InputActionID action);

    //* Returns true if input received
    bool takeInputMouse(bool isCursorActive);

    //* Returns true if input received
    bool takeInputKey();

    //* Returns true if input received
    bool takeInputGesture();

    InputActionID inputAction() const;
    void resetInputAction();
};

#endif