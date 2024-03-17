#include "Zoom.h"

#include "Constants.h"
#include <raylib.h>

namespace Zoom
{
    void update(float wheel, Camera2D& camera)
    {
        if (wheel != 0)
        {
            camera.zoom += (wheel * ZOOM_STEP);
            if (camera.zoom < ZOOM_STEP) camera.zoom = ZOOM_STEP;
        }
    }

    void reset(Camera2D& camera)
    {
        camera.zoom = 1.0f;
    }
}