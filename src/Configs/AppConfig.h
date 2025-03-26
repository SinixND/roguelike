#ifndef IG20240529010632
#define IG20240529010632

#include <raylib.h>

/// Configurations for the application
struct AppConfig
{
    //* Raylib settings
    Image const favicon{ LoadImage( "assets/favicon/favicon.png" ) };

    //* Window dimensions
    int windowWidth{ 800 };
    int windowHeight{ 940 };

    //* Fps
    int const fpsTarget{ 300 };
    bool showFPS{ true };
    bool vSync{ false };
};

#endif
