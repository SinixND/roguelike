#ifndef IG20250309192629
#define IG20250309192629

//* TODO: CHANGE/REMOVE
enum class GameState
{
    DEFAULT_OLD,
    LEVEL_UP_OLD,
    REGEN,
    //* Update single frame systems
    IDLE,
    //* Update multi frame systems
    BUSY,
    TURN_END,
    LEVEL_UP,
};

#endif
