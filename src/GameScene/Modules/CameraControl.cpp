#include "CameraControl.h"

#include "Directions.h"
#include "Panels.h"
#include "RuntimeDatabase.h"
#include "Textures.h"
#include "TileTransformation.h"
#include "Unit.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace
{
    void setPanDirection(Vector2I& panDirection, Vector2 screenCursor, RectangleEx edgePanRectangle)
    {
        if (screenCursor.x < edgePanRectangle.left())
        {
            panDirection += Directions::V_LEFT;
        }
        else if (screenCursor.x > (edgePanRectangle.right()))
        {
            panDirection += Directions::V_RIGHT;
        }

        if (screenCursor.y < edgePanRectangle.top())
        {
            panDirection += Directions::V_UP;
        }
        else if (screenCursor.y > (edgePanRectangle.bottom()))
        {
            panDirection += Directions::V_DOWN;
        }
    }
}

namespace CameraControl
{
    void edgePan(Camera2D& camera, Vector2 cursorWorldPosition, bool isMouseControlled)
    {
        static float dt{};
        dt += GetFrameTime();

        // Trigger if cursor is outside of edge pan deadzone
        Vector2 screenCursor{GetWorldToScreen2D(cursorWorldPosition, dtb::camera())};

        // Calculate edge pan deadzone (not triggered within)
        RectangleEx renderRectangle{PanelMap::panel()};

        RectangleEx edgePanDeadzone{
            Vector2AddValue(
                renderRectangle.topLeft(),
                EDGE_PAN_TRIGGER_WIDTH),
            Vector2SubtractValue(
                renderRectangle.bottomRight(),
                EDGE_PAN_TRIGGER_WIDTH)};

        // Check if cursor is inside edge pan deadzone
        if (CheckCollisionPointRec(screenCursor, edgePanDeadzone))
        {
            return;
        }

        // Only when mouse controlled: check if frame dt exceeds threshold
        // Has no effect if key controlled!
        if (isMouseControlled && dt < PAN_TICK)
        {
            return;
        }

        // Reset frame dt (only relevant if mouse contolled)
        dt = 0;

        // Adjust cursor position relative to edge pan deadzone while avoiding to pan too far
        Vector2I panDirection{Directions::V_NODIR};

        // Determine pan direction
        setPanDirection(panDirection, screenCursor, edgePanDeadzone);

        // Check if center of render area stays on map
        Vector2I renderCenter{
            TileTransformation::screenToPosition(
                Vector2{
                    renderRectangle.left() + (renderRectangle.width() / 2),
                    renderRectangle.top() + (renderRectangle.height() / 2)})};

        if (!CheckCollisionPointRectangleEx(
                Vector2Add(
                    renderCenter,
                    panDirection),
                dtb::mapsize()))
        {
            return;
        }

        // Update camera
        camera.target += Vector2Scale(panDirection, Textures::TILE_SIZE);
    }

    void centerOnHero(Camera2D& camera, Unit& unit)
    {
        camera.target = unit.transform.position();
    }
}
