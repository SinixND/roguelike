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
    bool executeAction(
        Hero* hero,
        Map* map,
        Cursor const& cursor,
        GameCamera const& gameCamera,
        InputId currentInput
    );
};

#endif
