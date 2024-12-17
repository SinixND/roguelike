#ifndef IG20240531194629
#define IG20240531194629

#include "InputActionId.h"
struct InputHandler;
struct Hero;
class Cursor;
struct Map;
class GameCamera;

namespace UserInputSystem
{
    void triggerAction(
        InputHandler& userInputComponent,
        Hero& hero,
        Cursor const& cursor,
        Map& map,
        GameCamera const& gameCamera);
};

#endif
