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
    void init(
        Scene& scene,
        AppConfig const& config
    );

    void update( Scene& scene );
    void deinit( Scene& scene );
}
#endif
