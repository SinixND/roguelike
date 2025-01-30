#ifndef IG20240531194629
#define IG20240531194629

enum class InputId;
class InputHandler;
struct Hero;
struct Cursor;
struct Map;
struct GameCamera;

namespace ActionSystem
{
    //* Return if multi-frame action taken
    bool takeAction(
        InputId currentInput,
        Hero& hero,
        Cursor const& cursor,
        Map& map,
        GameCamera const& gameCamera
    );
};

#endif
