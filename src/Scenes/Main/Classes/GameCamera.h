#ifndef IG20240531171525
#define IG20240531171525

#include "raylibEx.h"
#include <raylib.h>

//* Wrapper class for raylibs Camera2D
class GameCamera
{
    Camera2D camera_{};

    // Viewport points/equals to respective panel
    RectangleEx const* viewport_{};

public:
    void init(
        RectangleEx const& viewport,
        Vector2 const& heroPosition);

public:
    Camera2D const& camera() const;
    RectangleEx const& viewportOnScreen() const;
    RectangleExI viewportInTiles() const;

    //* Screen offset to draw target at
    void setOffset(Vector2 const& offset);

    //* World position to draw at offset
    void setTarget(Vector2 const& target);
    void setZoom(float zoom);

    void updateViewport(RectangleEx const& viewport);
};

#endif
