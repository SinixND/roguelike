#ifndef IG20240519210141
#define IG20240519210141

/**
 * @file
 * @brief Application layer handles frameworks and input
 * @details Default: raylib, extends to emscripten; Alternative: ncurses
 */

#include "AppConfig.h"
#include "InputHandler.h"
#include "InputId.h"
#include "InputMappings.h"
#include "Scene.h"
#include <raylib.h>

struct App
{
    float dt{ 0 };

    Cursor cursor{};

    InputId currentInputId{ InputId::NONE };
    InputMappings inputMappings;
    InputHandler inputHandler{};

    //* Scenes
    Scene scene{};
};

namespace AppModule
{
    void init(
        App& app,
        AppConfig const& config
    );

    void run( App& app );
    void deinit( Scene& scene );
}
#endif
