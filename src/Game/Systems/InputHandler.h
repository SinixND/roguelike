#ifndef IG20240531194629
#define IG20240531194629

#include "Cursor.h"
#include "GameCamera.h"
#include "Hero.h"
#include "InputActionID.h"
#include "Map.h"
#include <raylib.h>
#include <unordered_map>

// Maximum time for tap to trigger
double constexpr maxTapTime{0.3f};
// Minimum time for hold to trigger
double constexpr minHoldTime{0.5f};

class InputHandler
{
    InputActionID inputAction_{InputActionID::none};

    bool modifier_{};

    std::unordered_map<int, InputActionID> keyToInputActionID_{};
    std::unordered_map<int, InputActionID> mouseButtonToInputActionID_{};
    std::unordered_map<InputActionID, int> inputActionIDToModifierKey_{};

public:
    void setDefaultInputMappings();
    void takeInput();
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
    bool takeInputMouse();

    // Returns true if input received
    bool takeInputKey();

    // Returns true if input received
    bool takeInputGesture();
};

#endif
