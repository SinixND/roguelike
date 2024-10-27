#ifndef IG20231203204746
#define IG20231203204746

#include "Chunk.h"
#include "Cursor.h"
#include "DenseMap.h"
#include "Game.h"
#include "GameCamera.h"
#include "IScene.h"
#include "Panels.h"
#include "RenderSystem.h"
#include "VisibilitySystem.h"
#include "raylibEx.h"
#include <raylib.h>

class SceneMain : public snx::IScene
{
    Game game_{};

    Panels panels_{};
    GameCamera gameCamera_{};
    RenderSystem renderer_{};

    Cursor cursor_{};

    snx::DenseMap<Vector2I, Chunk> chunks_{};
    snx::DenseMap<Vector2I, Fog> fogs_{};

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
