#include "App.h"

#include "RNG.h"

//* Only define once
#define RAYGUI_IMPLEMENTATION
//* Required for custom icons set
#define RAYGUI_CUSTOM_ICONS

#include "../assets/iconset/iconset.rgi.h"
#include <raygui.h>

int main( /* int argc, char** argv */ )
{
#if defined( DEBUG )
    snx::RNG::seed( 1 );
#endif
    AppConfig config{};

    App app{};

    app = AppModule::init(
        app,
        config
    );

    AppModule::run( app );
    AppModule::deinit( app );

    return 0;
}

