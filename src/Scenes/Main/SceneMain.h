#ifndef IG20231203204746
#define IG20231203204746

#include "Chunk.h"
#include "Cursor.h"
#include "Data/RenderData.h"
#include "DenseMap.h"
#include "Game.h"
#include "GameCamera.h"
#include "Panels.h"
#include "RenderSystem.h"
#include "VisibilitySystem.h"
#include "raylibEx.h"
#include <raylib.h>

struct Scene
{
    Game game{};

    Panels panels{};
    GameCamera gameCamera{};
    RenderData renderData{};

    Cursor cursor{};

    snx::DenseMap<Vector2I, Chunk> chunks{};
    snx::DenseMap<Vector2I, Fog> fogs{};

#if defined(DEBUG)
    static inline Camera2D gcam{};
#endif
};

namespace SceneModule
{
    void init(Scene& scene);
    void update(Scene& scene);
    void deinitialize(Scene& scene);
    void processInput(Scene& scene);
    void updateState(Scene& scene);
    void renderOutput(Scene& scene);
    void setupSceneEvents(Scene& scene);
    void drawSceneBorder();
}
#endif
