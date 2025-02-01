#ifndef IG20240519210141
#define IG20240519210141

#include "AppConfig.h"
#include "InputHandler.h"
#include "InputId.h"
#include "Scene.h"
#include <raylib.h>

struct App
{
    //* Scenes
    Scene scene{};

    InputHandler inputHandler{};

    Cursor cursor{};

    InputId currentInputId{ InputId::NONE };

    float dt{ 0 };
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
