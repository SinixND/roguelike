#ifndef IG20240531171525
#define IG20240531171525

#include <raylib.h>

// Wrapper class for raylibs Camera2D
class GameCamera
{
    Camera2D camera_{};

public:
    void init(Vector2 const& offset);

    Camera2D const& get() const { return camera_; }

    // Screen offset to draw target at
    void setOffset(Vector2 const& offset);
    // World position to draw at offset
    void setTarget(Vector2 const& target);
    void setZoom(float zoom);
};

#endif