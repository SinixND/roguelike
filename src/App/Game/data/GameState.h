#ifndef IG20250309192629
#define IG20250309192629

//* TODO: CHANGE/REMOVE
enum class GameState
{
    DEFAULT_OLD,
    LEVEL_UP_OLD,
    REGEN,
    //* Update single frame systems
    BUSY,
    ACTION_HERO,
    ACTION_NPC,
    //* Update multi frame systems
    POST_ACTION,
    LEVEL_UP,
};

#endif
