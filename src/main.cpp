#include "App.h"
#include "AppConfigs.h"

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

    app.init( config );

    app.run();
    app.deinit();

    return 0;
}

