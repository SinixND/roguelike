#ifndef IG20241011025951
#define IG20241011025951

#include "Hero.h"
#include "UserInput.h"
#include "World.h"

class Cursor;
class GameCamera;

class Game
{
public:
    World world{};

    Hero hero{};

    UserInput userInput_{};

    bool actionInProgress_{false};

    //* Track game turns
    int turn_{0};

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
