#ifndef IG20250119163156
#define IG20250119163156

enum class InputId
{
    NONE = 0,
    //* Game actions
    ACT_UP,
    ACT_LEFT,
    ACT_DOWN,
    ACT_RIGHT,
    TARGET,
    ACT_IN_PLACE,
    //* UI
    TOGGLE_CURSOR,
    CYCLE_THEME,
    //* Input modifiers
    MOD,
};

#endif
