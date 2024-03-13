#include "CameraControl.h"

#include "Constants.h"
#include "Render.h"
#include "RuntimeDatabase.h"
#include "TileTransformation.h"
#include "Unit.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace
{
    void setPanDirection(Vector2& screenCursor, RectangleEx& edgePanArea, Vector2i& panDirection);
}

namespace CameraControl
{
    void edgePan(const Vector2& cursorWorldPosition, bool mouseActive)
    {
        static float dt{};
        dt += GetFrameTime();

        // Trigger if cursor is outside of edge pan area
        Vector2 screenCursor{GetWorldToScreen2D(cursorWorldPosition, dtb::camera())};

        RectangleEx renderArea{Render::getRenderArea()};

        RectangleEx edgePanArea{
            Vector2AddValue(
                renderArea.topLeft(),
                EDGE_PAN_FRAME_WEIGHT),
            Vector2SubtractValue(
                renderArea.bottomRight(),
                EDGE_PAN_FRAME_WEIGHT)};

        if (!CheckCollisionPointRec(screenCursor, edgePanArea))
        {
            if (mouseActive && dt < PAN_SPEED)
                return;

            dt = 0;

            // Adjust cursor position relative to edge pan area while avoiding to pan too far
            Vector2i panDirection{V_NODIR};

            // Determine pan direction
            setPanDirection(screenCursor, edgePanArea, panDirection);

            // Check if pan is valid
            Vector2i renderCenter{
                TileTransformation::screenToPosition(
                    Vector2{
                        renderArea.left() + (renderArea.width() / 2),
                        renderArea.top() + (renderArea.height() / 2)})};

            if (!CheckCollisionPointArea(
                    Vector2Add(
                        renderCenter,
                        panDirection),
                    dtb::mapSize()))
                return;

            // Update camera
            dtb::moveCamera(Vector2Scale(panDirection, TILE_SIZE));
        }
    }

    void centerOnHero(Camera2D& camera, Unit& unit)
    {
        camera.target = unit.position.get();
    }
}

namespace
{
    void setPanDirection(Vector2& screenCursor, RectangleEx& edgePanArea, Vector2i& panDirection)
    {
        if (screenCursor.x < edgePanArea.left())
        {
            panDirection = Vector2Add(panDirection, V_LEFT);
        }
        else if (screenCursor.x > (edgePanArea.right()))
        {
            panDirection = Vector2Add(panDirection, V_RIGHT);
        }

        if (screenCursor.y < edgePanArea.top())
        {
            panDirection = Vector2Add(panDirection, V_UP);
        }
        else if (screenCursor.y > (edgePanArea.bottom()))
        {
            panDirection = Vector2Add(panDirection, V_DOWN);
        }
    }
}