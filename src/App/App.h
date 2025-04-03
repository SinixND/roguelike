#ifndef IG20240519210141
#define IG20240519210141

#include "Cursor.h"
#include "Game.h"
#include "InputHandler.h"
#include "Screens.h"
#include <raylib.h>

struct AppConfig;

class App
{
public:
    enum class StateId
    {
        GAME_OVER,
        GAME_RUNNING,
    };

public:
    InputHandler inputHandler{};

    Cursor cursor{};
    float dt{ 0 };

    Screens screens{};
    Game game{};

    App::StateId stateId{ App::StateId::GAME_RUNNING };

public:
    void init( AppConfig const& config );

    void run();
    void deinit();

private:
    void setupAppEvents();
};
#endif
