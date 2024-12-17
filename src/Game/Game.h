#ifndef IG20241011025951
#define IG20241011025951

#include "Hero.h"
#include "InputHandler.h"
#include "InputMappings.h"
#include "World.h"

class Cursor;
class GameCamera;

class Game
{
public:
    World world{};

    Hero hero{};

    InputMappings inputMappings{};
    InputHandler inputHandler{};
    InputActionId inputAction{InputActionId::NONE};

    //* Track game turns
    int turn{0};

    size_t activeEnemyId{0};
    bool isMultiFrameActionActive{false};

public:
    void init();

    void processInput(Cursor& cursor);

    void updateState(
        GameCamera const& gameCamera,
        Cursor const& cursor);

    void setupGameEvents();
};

#endif
