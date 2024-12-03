#include "Cursor.h"
#include "EventId.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "Convert.h"
#include "raylibEx.h"
#include <raylib.h>

void Cursor::toggle()
{
    isActive = !isActive;

    if (isActive)
    {
        HideCursor();
        return;
    }

    ShowCursor();
}

void Cursor::update(Camera2D const& camera, Vector2I const& heroPosition)
{
    //* Update cursor position if active
    if (isActive)
    {
        Vector2I mouseTile{Convert::screenToTile(GetMousePosition(), camera)};

        if (!(position.tilePosition() == mouseTile))
        {
            position.changeTo(mouseTile);

            snx::PublisherStatic::publish(EventId::CURSOR_POSITION_CHANGED);
        }

        return;
    }

    //* Keep cursor on hero if inactive
    position.changeTo(heroPosition);
}
