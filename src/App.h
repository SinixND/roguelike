#ifndef IG20240519210141
#define IG20240519210141

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
    App init();
    void update( App& app );
    void deinit( App& app );
}
#endif
