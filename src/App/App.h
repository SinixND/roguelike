#ifndef IG20240519210141
#define IG20240519210141

#include "AppConfig.h"
#include "InputHandler.h"
#include "InputId.h"
#include "Scene.h"
#include <raylib.h>

struct App
{
    float dt{ 0 };

    Cursor cursor{};

    InputHandler inputHandler{};
    InputId currentInputId{ InputId::NONE };

    //* Scenes
    Scene scene{};
};

namespace AppModule
{
    [[nodiscard]]
    App const& init(
        App& app,
        AppConfig const& config
    );

    void run( App& app );
    void deinit( App& app );
}
#endif
