#ifndef IG20241011025951
#define IG20241011025951

#include "Hero.h"
#include "InputHandler.h"
#include "World.h"

// Externally provided
class Cursor;
class GameCamera;

class Game
{
    InputHandler inputHandler_{};
    World world_{};
    Hero hero_;

    bool actionsInProgress_{false};

    // Track game turns
    int turn_{0};

public:
    void init();

    void processInput(Cursor& cursor);
    void updateState(
        GameCamera const& gameCamera,
        Cursor const& cursor);

    void setupGameEvents();

    Hero const& hero() const;

    World const& world() const;
    World& world();
};

#endif