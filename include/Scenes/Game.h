#ifndef GAME_H_20231203204745
#define GAME_H_20231203204745

#include "Scene.h"

class GameScene
    : public snd::Scene
{
public:
    void initialize() override;
    void deinitialize() override;

private:
    void processInput() override;
    void updateState() override;
    void renderOutput(Camera2D& camera) override;
};
#endif
