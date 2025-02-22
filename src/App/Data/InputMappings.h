#ifndef IG20250118205709
#define IG20250118205709

#include "InputId.h"
#include <raylib.h>
#include <unordered_map>

struct InputMappings
{
    std::unordered_map<int, InputId> keyboardToInput{
        { KEY_NULL, InputId::NONE },
        { KEY_W, InputId::ACT_UP },
        { KEY_K, InputId::ACT_UP },
        { KEY_UP, InputId::ACT_UP },
        { KEY_A, InputId::ACT_LEFT },
        { KEY_H, InputId::ACT_LEFT },
        { KEY_LEFT, InputId::ACT_LEFT },
        { KEY_S, InputId::ACT_DOWN },
        { KEY_J, InputId::ACT_DOWN },
        { KEY_DOWN, InputId::ACT_DOWN },
        { KEY_D, InputId::ACT_RIGHT },
        { KEY_L, InputId::ACT_RIGHT },
        { KEY_RIGHT, InputId::ACT_RIGHT },
        { KEY_SPACE, InputId::ACT_IN_PLACE },
        { KEY_F2, InputId::CYCLE_THEME },
        { KEY_LEFT_SHIFT, InputId::MOD },
    };

    std::unordered_map<InputId, int> modifierToKeyboard{
        { InputId::MOD, KEY_LEFT_SHIFT },
    };

    std::unordered_map<int, InputId> mouseToInput{
        { MOUSE_BUTTON_RIGHT, InputId::TOGGLE_CURSOR },
        { MOUSE_BUTTON_LEFT, InputId::ACT_TO_TARGET },
    };
};

#endif
