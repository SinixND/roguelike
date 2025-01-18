#ifndef IG20240519210141
#define IG20240519210141

/**
 * @file
 * @brief Application layer handles frameworks and input
 * @details Default: raylib, extends to emscripten; Alternative: ncurses
 */

#include "AppConfig.h"
#include "Scene.h"
#include <raylib.h>

struct App
{
    AppConfig config{};

    //* Scenes
    Scene scene{};
};

namespace AppModule
{
    void run( App& app );
}
#endif
