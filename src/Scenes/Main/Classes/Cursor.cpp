#include "Cursor.h"

#include "Convert.h"
#include "EventId.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>

void CursorModule::toggle(Cursor& cursor)
{
    cursor.isActive = !cursor.isActive;

    if (cursor.isActive)
    {
        HideCursor();
        return;
    }

    ShowCursor();
}

void CursorModule::update(
    Cursor& cursor,
    Camera2D const& camera,
    Vector2I const& heroPosition)
{
    //* Update cursor position if active
    if (cursor.isActive)
    {
        Vector2I mouseTile{Convert::screenToTile(GetMousePosition(), camera)};

        if (!(PositionModule::tilePosition(cursor.position) == mouseTile))
        {
            PositionModule::changeTo(
                cursor.position,
                mouseTile);

            snx::PublisherStatic::publish(EventId::CURSOR_POSITION_CHANGED);
        }

        return;
    }

    //* Keep cursor on hero if inactive
    PositionModule::changeTo(
        cursor.position,
        heroPosition);
}
