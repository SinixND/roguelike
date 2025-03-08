#ifndef IG20240519210141
#define IG20240519210141

#include "AppConfig.h"
#include "AppState.h"
#include "Cursor.h"
#include "Game.h"
#include "InputHandler.h"
#include "InputId.h"
#include "Scenes.h"
#include <raylib.h>

struct App
{
    InputHandler inputHandler{};
    Cursor cursor{};
    float dt{ 0 };

    Scenes scenes{};
    Game game{};

    InputId currentInputId{ InputId::NONE };
    AppState state{ AppState::RUN_GAME };
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
