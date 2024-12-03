#ifndef IG20241011025951
#define IG20241011025951

#include "Hero.h"
#include "UserInputComponent.h"
#include "World.h"

class Cursor;
class GameCamera;

class Game
{
    UserInputComponent userInput_{};

    bool actionsInProgress_{false};

    //* Track game turns
    int turn_{0};

public:
    World world{};
    Hero hero{};

public:
    void init();

    void processInput(Cursor& cursor);
    void updateState(
        GameCamera const& gameCamera,
        Cursor const& cursor);

    void setupGameEvents();

    int turn() const;
};

#endif