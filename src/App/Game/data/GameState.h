#ifndef IG20250309192629
#define IG20250309192629

//* TODO: CHANGE/REMOVE
enum class GameState
{
    DEFAULT_OLD,
    LEVEL_UP,
    REGEN,
    //* Update single frame systems
    SINGLE_FRAME_ACTIONS,
    ACTION_HERO,
    ACTION_NPC,
    //* Update multi frame systems
    MULTI_FRAME_ACTIONS,
    POST_ACTION,
};

#endif
