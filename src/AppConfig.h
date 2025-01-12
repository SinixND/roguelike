#ifndef IG20240529010632
#define IG20240529010632

//* Configs imported from user file

#include <raylib.h>
struct AppConfig
{
    //* Window dimensions
    int windowWidth{ 800 };
    int windowHeight{ 940 };

    //* Fps
    int const fpsTarget{ 300 };
    bool showFPS{ true };
    bool vSync{ false };

    //* Raylib settings
    Image const favicon{ LoadImage( "assets/favicon/favicon.png" ) };
};

#endif
