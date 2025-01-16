#ifndef IG20241217212706
#define IG20241217212706

#include "InputActionId.h"
#include <raylib.h>
#include <unordered_map>

struct InputMappings
{
    std::unordered_map<int, InputActionId> keyboardToAction{
        { KEY_NULL, InputActionId::NONE },
        { KEY_W, InputActionId::ACT_UP },
        { KEY_K, InputActionId::ACT_UP },
        { KEY_UP, InputActionId::ACT_UP },
        { KEY_A, InputActionId::ACT_LEFT },
        { KEY_H, InputActionId::ACT_LEFT },
        { KEY_LEFT, InputActionId::ACT_LEFT },
        { KEY_S, InputActionId::ACT_DOWN },
        { KEY_J, InputActionId::ACT_DOWN },
        { KEY_DOWN, InputActionId::ACT_DOWN },
        { KEY_D, InputActionId::ACT_RIGHT },
        { KEY_L, InputActionId::ACT_RIGHT },
        { KEY_RIGHT, InputActionId::ACT_RIGHT },
        { KEY_SPACE, InputActionId::ACT_IN_PLACE }
    };

    std::unordered_map<int, InputActionId> mouseToAction{
        { MOUSE_BUTTON_RIGHT, InputActionId::TOGGLE_CURSOR },
        { MOUSE_BUTTON_LEFT, InputActionId::MOVE_TO_TARGET }
    };

    int modifierKey{ KEY_LEFT_SHIFT };
};

#endif
