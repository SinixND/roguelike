#ifndef IG20231203204746
#define IG20231203204746

#include "Chunk.h"
#include "Cursor.h"
#include "Data/RenderData.h"
#include "DenseMap.h"
#include "Game.h"
#include "GameCamera.h"
#include "InputId.h"
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

    snx::DenseMap<Vector2I, Chunk> chunks{};
    snx::DenseMap<Vector2I, Fog> fogs{};

#if defined( DEBUG )
    static inline Camera2D gcam{};
#endif
};

namespace SceneModule
{
    void init(
        Scene& scene,
        Cursor const& cursor
    );

    void update(
        Scene& scene,
        Cursor& cursor,
        InputId currentInputId,
        float dt
    );

    void deinitialize( Scene& scene );
}
#endif
