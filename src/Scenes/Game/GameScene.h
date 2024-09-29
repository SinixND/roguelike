#ifndef IG20231203204746
#define IG20231203204746

#include "Chunks.h"
#include "Cursor.h"
#include "GameCamera.h"
#include "Hero.h"
#include "IScene.h"
#include "InputHandler.h"
#include "Panels.h"
#include "Renderer.h"
#include "Visibility.h"
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

    Chunks tileChunks_{};
    Visibility visibility_{};

public:
    void initialize();
    void update() override;
    void deinitialize();

private:
    void processInput();
    void updateState();
    void renderOutput();
    void postOutput();

    void setupEvents();

#if defined(DEBUG)
public:
    static inline Camera2D gcam{};
#endif
};

#endif
