#ifndef _20240218031042
#define _20240218031042

#include "EdgePan.h"

#include "GameObject.h"
#include "RuntimeDatabase.h"
#include "TilePositionConversion.h"
#include "Unit.h"

void processEdgePan(GameObject& cursor, Unit& hero)
{
    static float dt{};
    dt += GetFrameTime();

    // Check if out of deadzone
    auto screenCursor{positionToScreen(cursor.position())};
    auto screenHero{positionToScreen(hero.position())};

    if (!CheckCollisionPointRec(screenCursor, dtb::Constants::cursorDeadzone()))
    {
        if (dtb::Globals::isMouseActivated() && dt < 0.1f)
            return;

        dt = 0;

        // Adjust cursor position relative to deadzone
        if (screenCursor.x < dtb::Constants::cursorDeadzone().x &&
            screenHero.x < dtb::Constants::cursorDeadzone().x + dtb::Constants::cursorDeadzone().width)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_LEFT,
                    TILE_SIZE));
        }

        if (screenCursor.x > (dtb::Constants::cursorDeadzone().x + dtb::Constants::cursorDeadzone().width) &&
            screenHero.x > dtb::Constants::cursorDeadzone().x)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_RIGHT,
                    TILE_SIZE));
        }

        if (screenCursor.y < dtb::Constants::cursorDeadzone().y &&
            screenHero.y < dtb::Constants::cursorDeadzone().y + dtb::Constants::cursorDeadzone().height)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_UP,
                    TILE_SIZE));
        }

        if (screenCursor.y > (dtb::Constants::cursorDeadzone().y + dtb::Constants::cursorDeadzone().height) &&
            screenHero.y > dtb::Constants::cursorDeadzone().y)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_DOWN,
                    TILE_SIZE));
        }
    }
}

#endif