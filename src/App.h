#ifndef IG20240519210141
#define IG20240519210141

#include "AppConfig.h"
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
    static inline AppConfig config{};

    int const fpsTarget_{300};

    // Raylib settings
    Image const favicon_{LoadImage("resources/favicon/favicon.png")};

    // Scenes
    GameScene gameScene_{};

    // Set default scene
    snx::IScene* activeScene_{&gameScene_};

public:
    static inline snx::Publisher<std::function<void()>> eventDispatcher{};

public:
    void init();
    void run();
    void deinit();
};

#endif
