#ifndef IG20240531194629
#define IG20240531194629

#include "InputActionId.h"
struct Input;
struct Hero;
struct Cursor;
struct Map;
struct GameCamera;

namespace UserInputSystem
{
    //* Return if multi-frame action taken
    bool takeAction(
        InputActionId inputAction,
        Hero& hero,
        Cursor const& cursor,
        Map& map,
        GameCamera const& gameCamera);
};

#endif
