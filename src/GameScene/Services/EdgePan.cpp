#include "EdgePan.h"

#include "Constants.h"
#include "RuntimeDatabase.h"
#include "TileTransformation.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace EdgePan
{
    void update(const Vector2& cursorWorldPosition, bool mouseActive)
    {
        static float dt{};
        dt += GetFrameTime();

        // Check if cursor is outside of edge pan area
        auto screenCursor{GetWorldToScreen2D(cursorWorldPosition, dtb::camera())};

        Rectangle edgePanArea{
            GetRectangle(
                Vector2AddValue(
                    Vector2{0, 0},
                    EDGE_PAN_FRAME_WEIGHT),
                Vector2SubtractValue(
                    Vector2{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())},
                    EDGE_PAN_FRAME_WEIGHT))};

        if (!CheckCollisionPointRec(screenCursor, edgePanArea))
        {
            if (mouseActive && dt < PAN_SPEED)
                return;

            dt = 0;

            // Adjust cursor position relative to edge pan area while avoiding to pan too far
            Vector2i panDirection{V_NODIR};

            // Determine pan direction
            if (screenCursor.x < edgePanArea.x)
            {
                panDirection = Vector2Add(panDirection, V_LEFT);
            }
            else if (screenCursor.x > (edgePanArea.x + edgePanArea.width))
            {
                panDirection = Vector2Add(panDirection, V_RIGHT);
            }

            if (screenCursor.y < edgePanArea.y)
            {
                panDirection = Vector2Add(panDirection, V_UP);
            }
            else if (screenCursor.y > (edgePanArea.y + edgePanArea.height))
            {
                panDirection = Vector2Add(panDirection, V_DOWN);
            }

            // Check if pan is valid
            if (!CheckCollisionPointArea(
                    Vector2Add(
                        TileTransformation::screenToPosition(
                            {GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f}),
                        panDirection),
                    dtb::mapSize()))
                return;

            // Update camera
            dtb::moveCamera(Vector2Scale(panDirection, TILE_SIZE));
        }
    }
}
