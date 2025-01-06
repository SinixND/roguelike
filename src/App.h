#ifndef IG20240519210141
#define IG20240519210141

#include "AppConfig.h"
#include "SceneMain.h"
#include <raylib.h>

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
