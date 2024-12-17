#ifndef IG20241217212706
#define IG20241217212706

#include "InputActionId.h"
#include <unordered_map>

struct InputMappings
{
    std::unordered_map<int, InputActionId> keyboardToAction{};
    std::unordered_map<int, InputActionId> mouseToAction{};
    int modifierKey{};
};

InputMappings defaultInputMappings();

#endif
