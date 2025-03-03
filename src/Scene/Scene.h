#ifndef IG20231203204746
#define IG20231203204746

#include "Chunk.h"
#include "DenseMap.h"
#include "GameCamera.h"
#include "Panels.h"
#include "RenderData.h"

struct Hero;
struct Map;
class World;
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
        World const& world
    );

    [[nodiscard]]
    Scene const& update(
        Scene& scene,
        Hero const& hero,
        World const& world,
        Cursor& cursor,
        InputId currentInputId
    );

    void deinitialize( Scene& scene );
}
#endif
