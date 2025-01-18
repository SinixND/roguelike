/**
 * @file
 * @brief Entrypoint
 */

#include "App.h"

//* Only define once
#define RAYGUI_IMPLEMENTATION
//* Required for custom icons set
#define RAYGUI_CUSTOM_ICONS

#include "../assets/iconset/iconset.rgi.h"
#include <raygui.h>

int main( /* int argc, char** argv */ )
{
    AppConfig config{};

    App app{};

    AppModule::init(
        app,
        config
    );

    SceneModule::init(
        app.scene,
        app.cursor
    );

    AppModule::run( app );
    AppModule::deinit( app.scene );

    return 0;
}
