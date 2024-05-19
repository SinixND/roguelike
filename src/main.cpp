#include "App.h"
// #include <raylib.h>
// #include <raymath.h>

#define RAYGUI_IMPLEMENTATION // Only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset/iconset.rgi.h"

#include <raygui.h>
// #include <raylibEx.h>

// #ifdef __EMSCRIPTEN__
// #include <emscripten/emscripten.h>
// #endif

int main(/* int argc, char **argv */)
{
    App application{};

    application.init();
    application.run();
    application.deinit();

    return 0;
}
