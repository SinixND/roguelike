#ifndef IG20240426003847
#define IG20240426003847

#include "IScene.h"

class ActiveScene
{
private:
    snx::IScene* activeScene_{};

public:
    snx::IScene* operator()() { return activeScene_; }
    void setActiveScene(snx::IScene& scene) { activeScene_ = &scene; }

    ActiveScene(snx::IScene& scene)
        : activeScene_(&scene)
    {
    }
};

#endif