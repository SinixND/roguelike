#ifndef IG20241011025951
#define IG20241011025951

#include "Hero.h"
#include "InputId.h"
#include "World.h"

struct Cursor;
struct GameCamera;

struct Game
{
    World world{};

    Hero hero{};

    //* Track game turns
    int turn{ 0 };

    size_t activeEnemyId{ 0 };
    bool isMultiFrameActionActive{ false };
};

namespace GameModule
{
    void init( Game& game );

    void setupGameEvents(
        Game& game
    );

    void update(
        Game& game,
        GameCamera const& gameCamera,
        Cursor const& cursor,
        InputId currentInputId,
        float dt
    );
}

#endif
