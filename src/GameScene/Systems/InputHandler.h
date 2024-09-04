#ifndef IG20240531194629
#define IG20240531194629

#include "Cursor.h"
#include "Hero.h"
#include "InputActionID.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>
#include <unordered_map>

class InputHandler
{
    InputActionID inputAction_{InputActionID::none};

    bool modifier_{};

    std::unordered_map<int, InputActionID> keyToInputActionID_{};
    std::unordered_map<int, InputActionID> mouseButtonToInputActionID_{};
    std::unordered_map<InputActionID, int> inputActionIDToModifierKey_{};

    void bindKey(int key, InputActionID action);
    void bindMouseButton(int key, InputActionID action);
    void bindModifierKey(int key, InputActionID action);

    // Returns true if input received
    bool takeInputMouse(bool isCursorActive);

    // Returns true if input received
    bool takeInputKey();

    // Returns true if input received
    bool takeInputGesture();

public:
    void setDefaultInputMappings();
    void takeInput(bool isCursorActive);
    void triggerAction(
        Hero& hero,
        Cursor& cursor,
        Tiles& map,
        RectangleEx const& mapPanel,
        Camera2D const& camera);
};

#endif
