#ifndef IG20240531194629
#define IG20240531194629

#include "Cursor.h"
#include "GameCamera.h"
#include "Hero.h"
#include "InputActionID.h"
#include "Map.h"
#include <raylib.h>
#include <unordered_map>

// Maximum hold time for tap event to trigger
double constexpr maxTapTime{0.3f};
// Minimum hold time for hold event to trigger
double constexpr minHoldTime{0.3f};
// Maximum time between taps for double tap event to trigger
double constexpr maxDoubleTapTime{0.3f};

class InputHandler
{
    InputActionID inputAction_{InputActionID::none};

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
    void setDefaultInputMappings();
    void takeInput(bool isCursorActive);
    void triggerAction(
        Hero& hero,
        Cursor const& cursor,
        Map const& map,
        GameCamera const& gameCamera);

private:
    void bindKey(int key, InputActionID action);
    void bindMouseButton(int key, InputActionID action);
    void bindModifierKey(int key, InputActionID action);

    // Returns true if input received
    bool takeInputMouse(bool isCursorActive);

    // Returns true if input received
    bool takeInputKey();

    // Returns true if input received
    bool takeInputGesture();
};

#endif
