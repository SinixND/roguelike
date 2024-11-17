#ifndef IG20241011025951
#define IG20241011025951

#include "Hero.h"
#include "UserInput.h"
#include "World.h"

class Cursor;
class GameCamera;

struct Game
{
    UserInput userInput_{};

    bool actionsInProgress_{false};

    //* Track game turns
    int turn_{0};

    World world{};
    Hero hero{};
};

void initGame(Game* game);

void processUserInput(
    Game* game,
    Cursor& cursor);

void updateGameState(
    Game* game,
    GameCamera const& gameCamera,
    Cursor const& cursor);

void setupGameEvents(Game* game);

#endif
