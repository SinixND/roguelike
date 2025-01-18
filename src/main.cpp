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
    App app{};

    AppModule::run( app );

    return 0;

    return 0;
}
