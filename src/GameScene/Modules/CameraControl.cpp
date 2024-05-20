#include "CameraControl.h"

#include "Directions.h"
#include "Event.h"
#include "Publisher.h"
#include "TextureData.h"
#include "TileTransformation.h"
#include "Unit.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace CameraControl
{
    void setPanDirection(Vector2I* panDirection, Vector2 screenCursor, RectangleEx edgePanRectangle)
    {
        if (screenCursor.x < edgePanRectangle.left())
        {
            *panDirection += Directions::V_LEFT;
        }
        else if (screenCursor.x > (edgePanRectangle.right()))
        {
            *panDirection += Directions::V_RIGHT;
        }

        if (screenCursor.y < edgePanRectangle.top())
        {
            *panDirection += Directions::V_UP;
        }
        else if (screenCursor.y > (edgePanRectangle.bottom()))
        {
            *panDirection += Directions::V_DOWN;
        }
    }

    bool isPanValid(RectangleEx const& renderRectangle, Camera2D const& camera, Vector2I* panDirection, RectangleExI const& mapRectangle)
    {
        Vector2I renderCenter{TileTransformation::screenToPosition(
            Vector2{renderRectangle.left() + (renderRectangle.width() / 2), renderRectangle.top() + (renderRectangle.height() / 2)},
            camera)};

        // Check x and y directions individually to enable half-valid pans
        if (!CheckCollisionPointRectangleEx(
                Vector2Add(renderCenter, Vector2I{panDirection->x, 0}),
                mapRectangle))
        {
            panDirection->x = 0;
        }

        if (!CheckCollisionPointRectangleEx(
                Vector2Add(renderCenter, Vector2I{0, panDirection->y}),
                mapRectangle))
        {
            panDirection->y = 0;
        }

        return Vector2Sum(*panDirection);
    }

    // Trigger if cursor is outside of edge pan deadzone
    void edgePan(Camera2D* camera, Vector2 cursorWorldPosition, bool isMouseControlled, RectangleExI const& mapArea, RectangleEx const& panelMap, Publisher& publisher)
    {
        static float dt{};
        dt += GetFrameTime();

        Vector2 screenCursor{GetWorldToScreen2D(cursorWorldPosition, *camera)};

        // Calculate edge pan deadzone (not triggered within)
        RectangleEx edgePanDeadzone{
            Vector2AddValue(panelMap.topLeft(), EDGE_PAN_TRIGGER_WIDTH),
            Vector2SubtractValue(panelMap.bottomRight(), EDGE_PAN_TRIGGER_WIDTH)};

        // Return if cursor is inside edge pan deadzone (not triggering)
        if (CheckCollisionPointRec(screenCursor, edgePanDeadzone))
        {
            return;
        }

        // Return if dt is below threshold for mouse controlled pan
        if (isMouseControlled && dt < PAN_TICK)
        {
            return;
        }

        // Reset frame dt (only relevant if mouse contolled)
        dt = 0;

        // Adjust cursor position relative to edge pan deadzone while avoiding to pan
        // too far Determine pan direction
        Vector2I panDirection{Directions::V_NODIR};
        setPanDirection(&panDirection, screenCursor, edgePanDeadzone);

        // Check if center of render area stays on map
        if (!isPanValid(panelMap, *camera, &panDirection, mapArea))
        {
            return;
        }

        // Update camera
        camera->target += Vector2Scale(panDirection, TextureData::TILE_SIZE);
        publisher.notify(Event::cameraChanged);
    }

    void centerOnHero(Camera2D* camera, Unit const& unit)
    {
        camera->target = unit.positionComponent.renderPosition();
    }

    void SubUpdateCameraOffset::onNotify()
    {
        camera_.offset = panelMap_.center();
    }
}
