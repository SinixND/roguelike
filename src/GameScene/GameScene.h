#ifndef IG20231203204746
#define IG20231203204746

#include "Cursor.h"
#include "GameCamera.h"
#include "Hero.h"
#include "IScene.h"
#include "InputHandler.h"
#include "Panels.h"
#include "Renderer.h"
#include "World.h"

class GameScene : public snx::IScene
{

    Panels panels_{};
    GameCamera camera_{};
    Renderer renderer_{};

    InputHandler inputHandler_{};

    World world_{};

    Cursor cursor_{};
    Hero hero_;

    bool actionInProgress_{false};

    // Filtered tiles for efficiency
    struct TilesToRender {
        std::vector<Vector2> renderPositions{};
        std::vector<RenderID> renderIDs{};
        std::vector<VisibilityID> visibilityIDs{};
    };

    TilesToRender tilesToRender_{};

    void processInput();
    void updateState();
    void renderOutput();
    void postOutput();

public:
    void initialize();
    void update() override;
    void deinitialize();

    std::vector<Vector2I> tilePositionsToRender();

    void initTilesToRender();

    // void updateTilesToRender();
};

#endif
