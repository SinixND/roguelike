#ifndef IG20240519210141
#define IG20240519210141

#include "AppConfig.h"
#include "SceneMain.h"
#include <raylib.h>

#if defined(EMSCRIPTEN)
struct emscriptenApp
{
    Scene* scene;
};
#endif

struct App
{
    AppConfig config{};

    int const fpsTarget{300};

    //* Raylib settings
    Image const favicon{LoadImage("resources/favicon/favicon.png")};

    //* Scenes
    Scene scene{};
};

namespace AppModule
{
    void init(App& app);
    void run(App& app);
    void deinit(App& app);
}
#endif
