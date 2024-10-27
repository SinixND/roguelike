#include "Cursor.h"

#include "Event.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "RenderID.h"
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
    if (isActive)
    {
        Vector2I mouseTile{UnitConversion::screenToTile(GetMousePosition(), camera)};

        if (!(position.tilePosition() == mouseTile))
        {
            position.changeTo(mouseTile);

            snx::PublisherStatic::publish(Event::CURSOR_POSITION_CHANGED);
        }

        return;
    }

    position.changeTo(heroPosition);
}
