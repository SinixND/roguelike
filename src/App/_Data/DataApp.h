#ifndef IG20250401144844
#define IG20250401144844

#include <raylib.h>

/// Configurations for the application
namespace Data::App
{
    Image const FAVICON{ LoadImage( "assets/favicon/favicon.png" ) };

    int constexpr WINDOE_WIDTH_MIN{ 640 };
    int constexpr WINDOE_HEIGHT_MIN{ 480 };

    int constexpr FPS_TARGET{ 300 };
};

#endif
