#ifndef IG20250305221255
#define IG20250305221255

#include "Chunk.h"
#include "DenseMap.h"
#include "GameCamera.h"
#include "GameScenePanels.h"
#include "RenderData.h"

struct Hero;
struct Map;
class World;
struct Cursor;
enum class InputId;

struct SceneGame
{
    SceneGamePanels panels{};
    snx::DenseMap<Vector2I, Chunk> chunks{};
    RenderData renderData{};
    GameCamera gameCamera{};
};

namespace SceneGameModule
{
    [[nodiscard]]
    SceneGame const& init(
        SceneGame& scene,
        Hero const& hero,
        World const& world
    );

    [[nodiscard]]
    SceneGame const& update(
        SceneGame& scene,
        Hero const& hero,
        World const& world,
        Cursor& cursor,
        InputId currentInputId
    );

    void deinitialize( SceneGame& scene );
}
#endif
