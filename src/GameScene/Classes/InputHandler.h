#ifndef IG20240531194629
#define IG20240531194629

#include "Hero.h"
#include "InputActionID.h"
#include <unordered_map>

class InputHandler
{
    InputActionID inputAction_{InputActionID::none};

    bool modifier_{};

    std::unordered_map<int, InputActionID> keyToInputActionID_{};
    std::unordered_map<InputActionID, int> inputActionIDToModifierKey_{};

    void bindKey(int key, InputActionID action);
    void bindModifierKey(int key, InputActionID action);
    bool takeInputKey();
    bool takeInputGesture();

public:
    void setDefaultInputMappings();
    void takeInput();
    void triggerAction(Hero& hero);
    void simulateInput(int key);
};

#endif
