#include "Cursor.h"
#include "Convert.h"
#include "EventId.h"
#include "PublisherStatic.h"
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

        if (!(Convert::worldToTile(position) == mouseTile))
        {
            position = Convert::tileToWorld(mouseTile);

            snx::PublisherStatic::publish(EventId::CURSOR_POSITION__CHANGED);
        }

        return;
    }

    //* Keep cursor on hero if inactive
    position = Convert::tileToWorld(heroPosition);
}
