#ifndef _20240218031042
#define _20240218031042

#include "EdgePan.h"

#include "GameObject.h"
#include "RuntimeDatabase.h"
#include "TilePositionConversion.h"
#include "Unit.h"
#include "raylibEx.h"

void processEdgePan(Vector2Int& cursorPosition, Vector2Int& keepInSightPosition)
{
    static float dt{};
    dt += GetFrameTime();

    // Check if out of deadzone
    auto screenCursor{positionToScreen(cursorPosition)};
    auto screenReference{positionToScreen(keepInSightPosition)};

    if (!CheckCollisionPointRec(screenCursor, dtb::Constants::cursorDeadzone()))
    {
        if (dtb::Configs::isMouseActivated() && dt < 0.1f)
            return;

        dt = 0;

        // Adjust cursor position relative to deadzone
        if (screenCursor.x < dtb::Constants::cursorDeadzone().x &&
            screenReference.x < dtb::Constants::cursorDeadzone().x + dtb::Constants::cursorDeadzone().width)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_LEFT,
                    TILE_SIZE));
        }

        if (screenCursor.x > (dtb::Constants::cursorDeadzone().x + dtb::Constants::cursorDeadzone().width) &&
            screenReference.x > dtb::Constants::cursorDeadzone().x)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_RIGHT,
                    TILE_SIZE));
        }

        if (screenCursor.y < dtb::Constants::cursorDeadzone().y &&
            screenReference.y < dtb::Constants::cursorDeadzone().y + dtb::Constants::cursorDeadzone().height)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_UP,
                    TILE_SIZE));
        }

        if (screenCursor.y > (dtb::Constants::cursorDeadzone().y + dtb::Constants::cursorDeadzone().height) &&
            screenReference.y > dtb::Constants::cursorDeadzone().y)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_DOWN,
                    TILE_SIZE));
        }
    }
}

#endif