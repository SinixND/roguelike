#ifndef GAME_H_20231203204745
#define GAME_H_20231203204745

#include "Renderer.h"
#include "Scene.h"

namespace snd
{

    class GameScene
        : public Scene
        , public Renderer
    //, public Controller
    {
    public:
        GameScene();

        void initialize() override;
        void deinitialize() override;

    private:
        Renderer renderer;
        // Controller controller;

        void processInput() override;
        void updateState() override;
        void renderOutput() override;
    };

}
#endif
