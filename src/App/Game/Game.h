#ifndef IG20241011025951
#define IG20241011025951

#include "GameState.h"
#include "Hero.h"
#include "InputId.h"
#include "World.h"

struct Cursor;
struct GameCamera;

struct Game
{
    Hero hero{};

    World world{};

    GameState state{ GameState::DEFAULT };

    //* Track game turns
    size_t turn{ 1 };

    size_t activeEnemyId{ 0 };
    int selectedAttribute{ 0 };
    bool isMultiFrameActionActive{ false };
};

namespace GameModule
{
    [[nodiscard]]
    Game const& init(
        Game& game,
        GameCamera const& gameCamera
    );

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
