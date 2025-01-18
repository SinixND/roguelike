#ifndef IG20241217212706
#define IG20241217212706

#include "ActionId.h"
#include "InputId.h"
#include <raylib.h>
#include <unordered_map>

struct ActionMappings
{
    std::unordered_map<InputId, ActionId> keyboardToAction{
        { InputId::NONE, ActionId::NONE },
        { InputId::UP, ActionId::ACT_UP },
        { InputId::LEFT, ActionId::ACT_LEFT },
        { InputId::DOWN, ActionId::ACT_DOWN },
        { InputId::RIGHT, ActionId::ACT_RIGHT },
        { InputId::IN_PLACE, ActionId::ACT_IN_PLACE }
    };

    std::unordered_map<int, ActionId> mouseToAction{
        { MOUSE_BUTTON_RIGHT, ActionId::TOGGLE_CURSOR },
        { MOUSE_BUTTON_LEFT, ActionId::MOVE_TO_TARGET }
    };

    int modifierKey{ KEY_LEFT_SHIFT };
};

#endif
