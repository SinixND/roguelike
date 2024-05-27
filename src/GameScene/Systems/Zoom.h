#ifndef IG20240308005429
#define IG20240308005429

#include <raylib.h>

namespace Zoom
{
    float constexpr ZOOM_DEFAULT{1};
    float constexpr ZOOM_MIN{0.1F};
    float constexpr ZOOM_MAX{3.0F};

    void update(float wheelMove, Camera2D* camera);
    void reset(Camera2D* camera);
}

#endif