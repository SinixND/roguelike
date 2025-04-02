#ifndef IG20240519210141
#define IG20240519210141

#include "AppStateId.h"
#include "Cursor.h"
#include "Game.h"
#include "InputHandler.h"
#include "Scenes.h"
#include <raylib.h>

struct AppConfig;

struct App
{
    InputHandler inputHandler{};

    Cursor cursor{};
    float dt{ 0 };

    Scenes scenes{};
    Game game{};

    AppStateId state{ AppStateId::RUN_GAME };
};

namespace AppModule
{
    [[nodiscard]]
    App const& init(
        App& app,
        AppConfig const& config
    );

    void run( App& appIO );
    void deinit( App& appIO );
}
#endif
