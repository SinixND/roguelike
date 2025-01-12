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

    //* Scenes
    SceneMain sceneMain{};
};

namespace AppModule
{
    App init();
    void update(App& app);
    void deinit(App& app);
}
#endif
