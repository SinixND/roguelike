#ifndef IG20250305221255
#define IG20250305221255

#include "Chunk.h"
#include "DenseMap.h"
#include "GamePanels.h"
#include "LevelUpOverlay.h"
#include "RenderData.h"

class World;
struct Map;
struct GameCamera;
struct Hero;
struct Cursor;
enum class InputId;
enum class GameState;

class GameScreen
{
    snx::DenseMap<Vector2I, Chunk> chunks{};
    RenderData renderData{};

    LevelUpOverlay levelUpOverlay{};

public:
    GamePanels panels{};

public:
    void init(
        World const& world,
        GameCamera const& gameCamera
    );

    void update(
        Game const& game,
        Cursor const& cursor,
        GameCamera const& gameCamera,
        InputId currentInputId
    );

    void deinit();

private:
    void setupScreenEvents(
        World const& world
    );

    void renderOutput(
        Hero const& hero,
        Map const& currentMap,
        int currentMapLevel,
        Cursor const& cursor,
        GameCamera const& gameCamera
    );
};

#endif
