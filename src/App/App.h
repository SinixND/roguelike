#ifndef IG20240519210141
#define IG20240519210141

#include "Cursor.h"
#include "Game.h"
#include "InputHandler.h"
#include "Scenes.h"
#include <raylib.h>

struct AppConfig;

class App
{
public:
    enum class StateId
    {
        GAME_OVER,
        RUN_GAME,
    };

public:
    InputHandler inputHandler{};

    Cursor cursor{};
    float dt{ 0 };

    Scenes scenes{};
    Game game{};

    App::StateId stateId{ App::StateId::RUN_GAME };

public:
    void init(
        AppConfig const& config
    );

    void run();
    void deinit();

private:
    void setupAppEvents();
};
#endif
