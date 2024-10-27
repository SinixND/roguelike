#ifndef IG20240719133200
#define IG20240719133200

enum class InputActionID
{
    NONE = 0,
    //* Actions
    ACT_UP,
    ACT_LEFT,
    ACT_DOWN,
    ACT_RIGHT,
    MOVE_TO_TARGET,
    ACT_IN_PLACE,
    //* Utility
    TOGGLE_CURSOR,
    //* Modifiers
    MOD,
};

#endif
