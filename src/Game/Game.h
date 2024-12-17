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

    bool actionInProgress_{false};

    //* Track game turns
    int turn_{0};

public:
    void init();

    void prepare(Cursor& cursor);

    void update(
        GameCamera const& gameCamera,
        Cursor const& cursor);

    void setupGameEvents();

    int turn() const;
};

#endif
