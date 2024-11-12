#include "Cursor.h"
#include "EventID.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "UnitConversion.h"
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
        Vector2I mouseTile{UnitConversion::screenToTile(GetMousePosition(), camera)};

        if (!(position.tilePosition() == mouseTile))
        {
            position.changeTo(mouseTile);

            snx::PublisherStatic::publish(EventID::CURSOR_POSITION_CHANGED);
        }

        return;
    }

    //* Keep cursor on hero if inactive
    position.changeTo(heroPosition);
}
