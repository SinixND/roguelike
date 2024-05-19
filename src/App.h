#ifndef IG20240519210141
#define IG20240519210141

#include "GameScene.h"
#include "IScene.h"
#include <raylib.h>

#ifdef __EMSCRIPTEN__
// Emscripten compatible app loop
//=====================================
struct emscriptenArgs
{
    snx::IScene* activeScene;
};
//=====================================
#endif

class App
{
public:
    void init();
    void run();
    void deinit();

private:
    // Application window
    int const WINDOW_WIDTH{800};
    int const WINDOW_HEIGHT{450};
    int const FPS_TARGET{245};

    // Raylib settings
    Image const favicon{LoadImage("resources/favicon/favicon.png")};

    // Scenes
    GameScene gameScene{};

    // Set default scene
    snx::IScene* activeScene{&gameScene};
};

#endif