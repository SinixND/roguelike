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

    RectangleExI viewportInTiles( GameCamera const& gameCamera );

    //* Screen offset to draw target at
    [[nodiscard]]
    Camera2D const& setOffset(
        Camera2D& camera,
        Vector2 const& offset
    );

    //* World position to draw at offset
    [[nodiscard]]
    Camera2D const& setTarget(
        Camera2D& camera,
        Vector2 const& target
    );

    [[nodiscard]]
    Camera2D const& setZoom(
        Camera2D& camera,
        float zoom
    );
}

#endif
