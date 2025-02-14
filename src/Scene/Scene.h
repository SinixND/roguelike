#ifndef IG20231203204746
#define IG20231203204746

#include "Chunk.h"
#include "DenseMap.h"
#include "GameCamera.h"
#include "Panels.h"
#include "RenderData.h"

struct Hero;
struct Map;
class Levels;
struct Cursor;
enum class InputId;

struct Scene
{
    Panels panels{};
    GameCamera gameCamera{};
    RenderData renderData{};

    snx::DenseMap<Vector2I, Chunk> chunks{};

#if defined( DEBUG )
    static inline Camera2D gcam{};
#endif
};

namespace SceneModule
{
    [[nodiscard]]
    Scene const& init(
        Scene& scene,
        Hero const& hero,
        Map& currentMap,
        Cursor const& cursor
    );

    [[nodiscard]]
    Scene const& update(
        Scene& scene,
        Hero const& hero,
        Levels const& levels,
        Cursor& cursor,
        InputId currentInputId
    );

    void deinitialize( Scene& scene );
}
#endif
