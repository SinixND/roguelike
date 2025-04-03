#ifndef IG20250305221255
#define IG20250305221255

#include "Chunk.h"
#include "DenseMap.h"
#include "Game.h"
#include "GameCamera.h"
#include "GamePanels.h"
#include "Overlays.h"
#include "RenderData.h"

struct Cursor;
enum class InputId;

class GameScreen
{
    snx::DenseMap<Vector2I, Chunk> chunks{};
    RenderData renderData{};

    Overlays overlays{};

public:
    GamePanels panels{};
    GameCamera gameCamera{};

public:
    void init( Game const& game );

    void update(
        Game const& game,
        Cursor const& cursor,
        InputId currentInputId
    );

    void deinit();

private:
    void setupScreenEvents( Game const& game );
    void renderOutput(
        Game const& game,
        Cursor const& cursor
    );
};

#endif
