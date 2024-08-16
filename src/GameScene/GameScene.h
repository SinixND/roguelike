#ifndef IG20231203204746
#define IG20231203204746

#include "ChunksToRender.h"
#include "Cursor.h"
#include "GameCamera.h"
#include "Hero.h"
#include "IScene.h"
#include "InputHandler.h"
#include "Panels.h"
#include "Renderer.h"
#include "World.h"
#include <raylib.h>

class GameScene : public snx::IScene
{
    Panels panels_{};
    GameCamera gameCamera_{};
    Renderer renderer_{};

    InputHandler inputHandler_{};

    World world_{};

    Cursor cursor_{};
    Hero hero_;

    bool actionInProgress_{false};

    ChunksToRender chunksToRender_{};
    // VisibilityToRender VisibilityToRender_{};

private:
    void processInput();
    void updateState();
    void renderOutput();
    void postOutput();

    // RectangleExI renderRectangleExI();

public:
    void initialize();
    void update() override;
    void deinitialize();
};

#endif
