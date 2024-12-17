#include "InputMappings.h"

#include <raylib.h>

InputMappings defaultInputMappings()
{
    InputMappings inputMappings{};

    inputMappings.keyboardToAction.insert({KEY_NULL, InputActionId::NONE});

    inputMappings.keyboardToAction.insert({KEY_W, InputActionId::ACT_UP});
    inputMappings.keyboardToAction.insert({KEY_K, InputActionId::ACT_UP});
    inputMappings.keyboardToAction.insert({KEY_UP, InputActionId::ACT_UP});

    inputMappings.keyboardToAction.insert({KEY_A, InputActionId::ACT_LEFT});
    inputMappings.keyboardToAction.insert({KEY_H, InputActionId::ACT_LEFT});
    inputMappings.keyboardToAction.insert({KEY_LEFT, InputActionId::ACT_LEFT});

    inputMappings.keyboardToAction.insert({KEY_S, InputActionId::ACT_DOWN});
    inputMappings.keyboardToAction.insert({KEY_J, InputActionId::ACT_DOWN});
    inputMappings.keyboardToAction.insert({KEY_DOWN, InputActionId::ACT_DOWN});

    inputMappings.keyboardToAction.insert({KEY_D, InputActionId::ACT_RIGHT});
    inputMappings.keyboardToAction.insert({KEY_L, InputActionId::ACT_RIGHT});
    inputMappings.keyboardToAction.insert({KEY_RIGHT, InputActionId::ACT_RIGHT});

    inputMappings.keyboardToAction.insert({KEY_SPACE, InputActionId::ACT_IN_PLACE});

    inputMappings.modifierKey = KEY_LEFT_SHIFT;

    inputMappings.mouseToAction.insert({MOUSE_BUTTON_RIGHT, InputActionId::TOGGLE_CURSOR});
    inputMappings.mouseToAction.insert({MOUSE_BUTTON_LEFT, InputActionId::MOVE_TO_TARGET});

    return inputMappings;
}
