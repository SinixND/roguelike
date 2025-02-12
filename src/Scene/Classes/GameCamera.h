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
    [[nodiscard]]
    GameCamera const& init(
        GameCamera& gameCamera,
        RectangleEx const& viewport,
        Vector2 const& heroPosition
    );

    RectangleExI viewportInTiles(
        GameCamera& gameCamera
    );

    //* Screen offset to draw target at
    [[nodiscard]]
    GameCamera const& setOffset(
        GameCamera& gameCamera,
        Vector2 const& offset
    );

    //* World position to draw at offset
    [[nodiscard]]
    GameCamera const& setTarget(
        GameCamera& gameCamera,
        Vector2 const& target
    );

    [[nodiscard]]
    GameCamera const& setZoom(
        GameCamera& gameCamera,
        float zoom
    );
}

#endif
