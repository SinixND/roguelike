#include "Zoom.h"
#include "Constants.h"
#include "RuntimeDatabase.h"
#include <raylib.h>

void processZoom()
{
    Camera2D& camera{dtb::Globals::camera()};
    float wheel = GetMouseWheelMove();

    if (wheel != 0)
    {
        camera.zoom += (wheel * ZOOM_STEP);
        if (camera.zoom < ZOOM_STEP) camera.zoom = ZOOM_STEP;
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && (IsKeyPressed(KEY_KP_0) || IsKeyPressed(KEY_ZERO)))
    {
        camera.zoom = 1.0f;
    }
};