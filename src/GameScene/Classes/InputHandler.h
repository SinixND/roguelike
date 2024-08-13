#ifndef IG20240531194629
#define IG20240531194629

#include "Hero.h"
#include "InputActionID.h"
#include <unordered_map>

class InputHandler
{
    InputActionID inputAction_{InputActionID::none};
    std::unordered_map<int, InputActionID> keyToInputActionID_{};

    void bind(int key, InputActionID action);
    bool takeInputKey();
    bool takeInputGesture();

public:
    void setDefaultInputMappings();
    void takeInput();
    void triggerInput(Hero& hero);
};

#endif
