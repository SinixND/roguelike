#ifndef _20231203204745
#define _20231203204745

#include "GameObject.h"
#include "Scene.h"
#include "Unit.h"
#include "World.h"

class GameScene
    : public snd::Scene
{
public:
    void initialize() override;
    void deinitialize() override;

private:
    World world{};
    GameObject cursor{};
    Unit hero{};

private:
    void processInput() override;
    void updateState() override;
    void renderOutput() override;
    void postOutput() override;
};
#endif
