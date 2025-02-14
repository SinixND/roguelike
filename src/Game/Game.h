#ifndef IG20241011025951
#define IG20241011025951

#include "Hero.h"
#include "InputId.h"
#include "Levels.h"

struct Cursor;
struct GameCamera;

struct Game
{
    Levels levels{};

    Hero hero{};

    //* Track game turns
    int turn{ 0 };

    size_t activeEnemyId{ 0 };
    bool isMultiFrameActionActive{ false };
};

namespace GameModule
{
    [[nodiscard]]
    Game const& init( Game& game );

    [[nodiscard]]
    Game const& update(
        Game& game,
        GameCamera const& gameCamera,
        Cursor const& cursor,
        InputId currentInputId,
        float dt
    );
}

#endif
