#ifndef IG20240531171525
#define IG20240531171525

#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

// Wrapper class for raylibs Camera2D
class GameCamera
{
    Camera2D camera_{};
    RectangleEx const* viewport_{};

public:
    void init(
        RectangleEx const& viewport,
        Vector2 const& heroPosition);

    Camera2D const& camera() const
    {
        return camera_;
    }

    RectangleEx const& viewportOnScreen() const
    {
        return *viewport_;
    }

    RectangleExI const viewportInTiles() const
    {
        return RectangleExI{
            UnitConversion::screenToTile(
                viewport_->topLeft(),
                camera_),
            UnitConversion::screenToTile(
                viewport_->bottomRight(),
                camera_)};
    }

    // Screen offset to draw target at
    void setOffset(Vector2 const& offset);

    // World position to draw at offset
    void setTarget(Vector2 const& target);
    void setZoom(float zoom);

    void updateViewport(RectangleEx const& viewport);
};

#endif
