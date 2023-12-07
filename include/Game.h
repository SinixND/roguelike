#ifndef GAME_H_20231203204745
#define GAME_H_20231203204745

#include "Character.h"
#include "Render.h"
#include "Scene.h"

namespace snd
{

    class Game
        : public Scene
        , public Renderer
    {
    public:
        Game();

        void initialize() override;
        void deinitialize() override;

    private:
        Character player_;

        void processInput() override;
        void updateState() override;
        void renderOutput() override;
    };

}
#endif