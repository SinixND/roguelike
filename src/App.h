#ifndef IG20240519210141
#define IG20240519210141

#include "AppConfig.h"
#include "IScene.h"
#include "SceneMain.h"
#include <raylib.h>

#if defined(EMSCRIPTEN)
struct emscriptenArgs
{
    snx::IScene* activeScene;
};
#endif

class App
{
    static inline AppConfig config{};

    int const fpsTarget_{300};

    //* Raylib settings
    Image const favicon_{LoadImage("resources/favicon/favicon.png")};

    //* Scenes
    SceneMain SceneMain_{};

    //* Set default scene
    snx::IScene* activeScene_{&SceneMain_};

public:
    void init();
    void run();
    void deinit();
};

#endif
