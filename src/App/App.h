#ifndef IG20240519210141
#define IG20240519210141

#include "Game.h"
#include "GameCamera.h"
#include "Input.h"
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
    Game game{};
    Input input{};
    float dt{ 0 };

    Screens screens{};
    GameCamera gameCamera{};

    App::StateId stateId{ App::StateId::GAME_RUNNING };

public:
    void init( AppConfig const& config );

    void run();
    void deinit();

private:
    void setupAppEvents();
};
#endif
