#ifndef IG20231203204746
#define IG20231203204746

#include "Chunk.h"
#include "Cursor.h"
#include "Data/RenderData.h"
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
    RenderData renderData_{};

    Cursor cursor_{};

    snx::DenseMap<Vector2I, Chunk> chunks_{};
    snx::DenseMap<Vector2I, Fog> fogs_{};

public:
    void init();
    void run() override;
    void deinitialize();

private:
    void prepare();
    void update();
    void render();

    void setupSceneEvents();

#if defined(DEBUG)
public:
    static inline Camera2D gcam{};
#endif
};

#endif
