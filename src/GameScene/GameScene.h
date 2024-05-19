#ifndef IG20231203204746
#define IG20231203204746

#include "CheatMode.h"
#include "GameFont.h"
#include "GameObject.h"
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
    Textures textures_{};
    GameFont gameFont_{};

    RectangleEx panelTileInfo{};
    RectangleEx panelInfo{};
    RectangleEx panelStatus{};
    RectangleEx panelLog{};
    RectangleEx panelMap{};

    Camera2D camera_{};

    GameObject cursor_{
        PositionComponent(),
        GraphicComponent(
            RenderID::CURSOR,
            LayerID::UI)};

    World gameWorld_{};

    bool isInputBlocked_{false};

public:
    void initialize() override;
    void update() override;
    void deinitialize() override;

private:
    void processInput() override;
    void updateState() override;
    void renderOutput() override;
    void postOutput() override;
};

#endif
