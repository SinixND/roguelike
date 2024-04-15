#include "Zoom.h"

#include <raylib.h>

namespace Zoom
{
    void update(float wheel, Camera2D& camera)
    {
        // camera.zoom += (wheel * ZOOM_STEP); // 0.125F

        // No zoom
        if (wheel == 0)
        {
            return;
        }

        // Zoom in
        if (wheel > 0)
        {
            if (camera.zoom < ZOOM_DEFAULT)
            {
                camera.zoom = ZOOM_DEFAULT;
            }
            else
            {
                camera.zoom = ZOOM_MAX;
            }
        }

        // Zoom out
        else
        {
            if (camera.zoom > ZOOM_DEFAULT)
            {
                camera.zoom = ZOOM_DEFAULT;
            }
            else
            {
                camera.zoom = ZOOM_MIN;
            }
        }
    }

    void reset(Camera2D& camera)
    {
        camera.zoom = 1.0F;
    }
}