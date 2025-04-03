#ifndef IG20250305221255
#define IG20250305221255

#include "Chunk.h"
#include "DenseMap.h"
#include "GameCamera.h"
#include "GamePanels.h"
#include "GameState.h"
#include "Overlays.h"
#include "RenderData.h"

struct Hero;
struct Map;
class World;
struct Cursor;
enum class InputId;

struct ScreenGame
{
    GamePanels panelComponents{};
    snx::DenseMap<Vector2I, Chunk> chunks{};
    RenderData renderData{};
    GameCamera gameCamera{};
    Overlays overlays{};
};

namespace ScreenGameModule
{
    [[nodiscard]]
    ScreenGame const& init(
        ScreenGame& screen,
        Hero const& hero,
        World const& world
    );

    [[nodiscard]]
    ScreenGame const& update(
        ScreenGame& gameScreen,
        Hero& hero,
        World const& world,
        Cursor const& cursor,
        GameState gameState,
        InputId currentInputId
    );

    void deinitialize( ScreenGame& screen );
}
#endif
