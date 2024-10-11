#ifndef IG20231203204746
#define IG20231203204746

#include "Chunks.h"
#include "Cursor.h"
#include "Game.h"
#include "GameCamera.h"
#include "IScene.h"
#include "Panels.h"
#include "Renderer.h"
#include "Visibility.h"
#include <raylib.h>

class SceneMain : public snx::IScene
{
    Game game_{};

    Panels panels_{};
    GameCamera gameCamera_{};
    Renderer renderer_{};

    Cursor cursor_{};

    Chunks tileChunks_{};
    Visibility visibility_{};

public:
    void init();
    void update() override;
    void deinitialize();

private:
    void processInput();
    void updateState();
    void renderOutput();

    void setupSceneEvents();

#if defined(DEBUG)
public:
    static inline Camera2D gcam{};
#endif
};

#endif
