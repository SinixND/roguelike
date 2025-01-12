#ifndef IG20241011025951
#define IG20241011025951

#include "Hero.h"
#include "Input.h"
#include "InputMappings.h"
#include "World.h"

struct Cursor;
struct GameCamera;

struct Game
{
    World world{};

    Hero hero{};

    InputMappings inputMappings{};
    Input inputHandler{};
    InputActionId inputAction{InputActionId::NONE};

    //* Track game turns
    int turn{0};

    size_t activeEnemyId{0};
    bool isMultiFrameActionActive{false};
};

namespace GameModule
{
    void init(Game& game);

    void processInput(
        Game& game,
        Cursor& cursor);

    void updateState(
        Game& game,
        GameCamera const& gameCamera,
        Cursor const& cursor);

    void setupGameEvents(
        Game& game);
}

#endif
