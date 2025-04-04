#ifndef IG20250305221255
#define IG20250305221255

#include "Chunk.h"
#include "DenseMap.h"
#include "Game.h"
#include "GameCamera.h"
#include "GamePanels.h"
#include "OverlayLevelUp.h"
#include "RenderData.h"

struct Cursor;
enum class InputId;

class GameScreen
{
    snx::DenseMap<Vector2I, Chunk> chunks{};
    RenderData renderData{};

    LevelUpOverlay levelUpOverlay{};

public:
    GamePanels panels{};

public:
    void init(
        Hero const& hero,
        Map const& currentMap,
        int currentMapLevel
    );

    void update(
        Hero const& hero,
        Map const& currentMap,
        int currentMapLevel,
        Cursor const& cursor,
        InputId currentInputId
    );

    void deinit();

private:
    void setupScreenEvents(
        Hero const& hero,
        Map const& currentMap,
        int currentMapLevel
    );

    void renderOutput(
        Hero const& hero,
        Map const& currentMap,
        int currentMapLevel,
        Cursor const& cursor
    );
};

#endif
