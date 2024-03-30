#ifndef IG20231203204746
#define IG20231203204746

#include "IScene.h"

class GameScene
    : public snd::IScene
{
public:
    void initialize() override;
    void deinitialize() override;

private:
    void processInput() override;
    void updateState() override;
    void renderOutput() override;
    void postOutput() override;
};

#endif
