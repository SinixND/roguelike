#ifndef IG20231203204746
#define IG20231203204746

#include "Cursor.h"
#include "GameCamera.h"
#include "Hero.h"
#include "IScene.h"
#include "InputHandler.h"
#include "Panels.h"
#include "Renderer.h"

class GameScene : public snx::IScene
{

    Panels panels_{};
    GameCamera camera_{};
    Renderer renderer_{};

    InputHandler inputHandler_{};

    Cursor cursor_{};
    Hero hero_;

    bool actionInProgress_{false};

public:
    void initialize();
    void update() override;
    void deinitialize();

private:
    void processInput();
    void updateState();
    void renderOutput();
    void postOutput();
};

#endif
