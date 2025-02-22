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
    snx::DenseMap<Vector2I, Chunk> chunks{};
    RenderData renderData{};
    GameCamera gameCamera{};
};

namespace SceneModule
{
    [[nodiscard]]
    Scene const& init(
        Scene& scene,
        Hero const& hero,
        Levels const& levels
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
