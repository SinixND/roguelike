#ifndef IG20240531171525
#define IG20240531171525

#include "raylibEx.h"
#include <raylib.h>

//* Wrapper class for raylibs Camera2D
struct GameCamera
{
    Camera2D camera{};

    //* Viewport points/equals to respective panel
    RectangleEx const* viewport{};
};

namespace GameCameraModule
{
    void init(
        GameCamera& gameCamera,
        RectangleEx const& viewport,
        Vector2 const& heroPosition);

    RectangleExI viewportInTiles(
        GameCamera& gameCamera);

    //* Screen offset to draw target at
    void setOffset(
        GameCamera& gameCamera,
        Vector2 const& offset);

    //* World position to draw at offset
    void setTarget(
        GameCamera& gameCamera,
        Vector2 const& target);

    void setZoom(
        GameCamera& gameCamera,
        float zoom);

    void updateViewport(
        GameCamera& gameCamera,
        RectangleEx const& viewport);
}

#endif
