#ifndef IG20240426003927
#define IG20240426003927

#include <raylib.h>

class GameCamera
{
private:
    Camera2D camera_{};

public:
    auto operator()() -> Camera2D const& { return camera_; }

    void updateCameraOffset(Camera2D& camera)
    {
        camera.offset = {static_cast<int>(GetRenderWidth()) * 0.5F, GetRenderHeight() * 0.5F};
    }
};

#endif
