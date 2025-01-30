#ifndef IG20240519210141
#define IG20240519210141

#include "AppConfig.h"
#include "InputHandler.h"
#include "InputId.h"
#include "Scene.h"
#include <raylib.h>

class App
{
public:
    //* Scenes
    Scene scene{};

    Cursor cursor{};

    InputId currentInputId{ InputId::NONE };

    float dt{ 0 };

public:
    void init( AppConfig const& config );
    void run();
    void deinit();

private:
    InputHandler inputHandler{};
};

#endif
