#ifndef IG20231203204746
#define IG20231203204746

#include "CheatMode.h"
#include "Directions.h"
#include "GameFont.h"
#include "GameObject.h"
#include "GamePhase.h"
#include "Graphic.h"
#include "IScene.h"
#include "LayerID.h"
#include "Position.h"
#include "RenderID.h"
#include "Textures.h"
#include "World.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

class GameScene : public snx::IScene
{
private:
    const float BORDER_WIDTH{1};
    const Color BORDER_COLOR{GRAY};
    const Color BACKGROUND_COLOR{BLACK};

    CheatMode cheatMode_{};

    Camera2D camera_{
        Vector2Scale(GetDisplaySize(), 0.5f),
        Directions::V_NULL,
        0,
        1};

    Textures textures_{};

    GameFont gameFont_{};

    World gameWorld_{};

    GameObject cursor_{
        PositionComponent(),
        GraphicComponent(
            RenderID::CURSOR,
            LayerID::UI)};

    GamePhase gamePhase_{};

    bool isInputBlocked_{false};

public:
    void initialize() override;
    void update() override;
    void deinitialize() override;

    GameScene()
    {
        initialize();
    }

    void processInput() override;
    void updateState() override;
    void renderOutput() override;
    void postOutput() override;
};

#endif
