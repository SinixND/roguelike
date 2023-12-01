#ifndef SCENES_H_20231201203725
#define SCENES_H_20231201203725

#include <raylib.h>

typedef enum
{
    GAME,
} Scenes;

extern Scenes currentScene;

class Scene
{
public:
    Scene();
    void Initialize();
    virtual void Update();

protected:
    void ProcessInput();
    void UpdateState();
    void RenderOutput();
};

class Game : public Scene
{
public:
    Game();

    void Initialize();
    void Update() override;

private:
    void ProcessInput();
    void UpdateState();
    void RenderOutput();

    void RenderStatusbar();
    void RenderBody();
    void RenderPauseOverlay();
};
//-------------------------------------
#endif