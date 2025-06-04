#ifndef IG20241011025951
#define IG20241011025951

#include "GameState.h"
#include "Hero.h"
#include "World.h"

struct Cursor;
struct GameCamera;
enum class InputId;

struct Game
{
    Hero hero{};
    World world{};
    size_t turn{ 1 };

    GameState state{ GameState::REGEN };

    size_t activeEnemyId{ 0 };
    int selectedAttribute{ 0 };
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
