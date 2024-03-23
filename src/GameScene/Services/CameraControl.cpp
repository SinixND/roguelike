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
    void setPanDirection(
      Vector2i& panDirection,
      Vector2 screenCursor,
      RectangleEx const& edgePanArea)
    {

        // Pan left or right
        if (screenCursor.x < edgePanArea.left()) {
            addToVector2(panDirection, V_LEFT);
        }

        else if (screenCursor.x > (edgePanArea.right())) {
            addToVector2(panDirection, V_RIGHT);
        }

        // Pan up or down
        if (screenCursor.y < edgePanArea.top()) {
            addToVector2(panDirection, V_UP);
        }

        else if (screenCursor.y > (edgePanArea.bottom())) {
            addToVector2(panDirection, V_DOWN);
        }
    }

}

namespace CameraControl
{
    void edgePan(Vector2 cursorWorldPosition, bool isMouseActive)
    {
        static float dt{};
        dt += GetFrameTime();

        // Trigger if cursor is outside of edge pan area
        Vector2 screenCursor{
          GetWorldToScreen2D(cursorWorldPosition, dtb::camera())};
        RectangleEx renderArea{Render::getRenderArea()};

        RectangleEx edgePanArea{
          Vector2AddValue(renderArea.topLeft(), EDGE_PAN_FRAME_WEIGHT),
          Vector2SubtractValue(
            renderArea.bottomRight(),
            EDGE_PAN_FRAME_WEIGHT)};
        if (CheckCollisionPointRec(screenCursor, edgePanArea)) return;

        // Process edge pan
        if (isMouseActive && dt < PAN_TICK) return;
        dt = 0;

        // Adjust cursor position relative to edge pan area while avoiding
        // to pan too far
        Vector2i panDirection{V_NODIR};

        // Determine pan direction
        setPanDirection(panDirection, screenCursor, edgePanArea);

        // Check if center would go out of render area
        Vector2i renderCenter{TileTransformation::screenToPosition(Vector2{
          renderArea.left() + (renderArea.width() / 2),
          renderArea.top() + (renderArea.height() / 2)})};

        if (!CheckCollisionPointArea(
              Vector2Add(renderCenter, panDirection),
              dtb::mapSize()))
            return;

        // Update camera
        dtb::moveCamera(Vector2Scale(panDirection, TILE_SIZE));
    }

    void centerOnHero(Camera2D& camera, Unit& unit)
    {
        camera.target = unit.transform.position();
    }

}