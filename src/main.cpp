#include "App.h"

//* Only define once
#define RAYGUI_IMPLEMENTATION
//* Required for custom icons set
#define RAYGUI_CUSTOM_ICONS

#include "../resources/iconset/iconset.rgi.h"
#include <raygui.h>

int main(/* int argc,
char** argv */
)
{
    App app{};

    app.init();
    app.run();
    app.deinit();

    return 0;
}
