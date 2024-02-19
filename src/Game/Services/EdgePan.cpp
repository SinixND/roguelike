#ifndef _20240218031042
#define _20240218031042

#include "EdgePan.h"

#include "Constants.h"
#include "RuntimeDatabase.h"
#include <raylib.h>

void processEdgePan(const Vector2& cursorWorldPosition, const Vector2& referenceWorldPosition)
{
    static float dt{};
    dt += GetFrameTime();

    // Check if out of deadzone
    auto screenCursor{GetWorldToScreen2D(cursorWorldPosition, dtb::Globals::camera())};
    auto screenReference{GetWorldToScreen2D(referenceWorldPosition, dtb::Globals::camera())};

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