#ifndef IG20240531194629
#define IG20240531194629

enum class InputId;
struct Hero;
struct Cursor;
struct Map;
struct GameCamera;

namespace ActionSystem
{
    //* Return if multi-frame action taken
    bool executeAction(
        Hero& heroIO,
        Map& mapIO,
        Cursor const& cursor,
        GameCamera const& gameCamera,
        InputId currentInput
    );
};

#endif
