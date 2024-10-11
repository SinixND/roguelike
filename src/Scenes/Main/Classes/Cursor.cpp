#include "Cursor.h"

#include "Event.h"
#include "Position.h"
#include "PublisherStatic.h"
#include "RenderID.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

RenderID Cursor::renderID() const
{
    return renderID_;
}

bool Cursor::isActive() const
{
    return isActive_;
}

void Cursor::toggle()
{
    isActive_ = !isActive_;

    if (isActive_)
    {
        HideCursor();
        return;
    }

    ShowCursor();
}

void Cursor::update(Camera2D const& camera, Vector2I const& heroPosition)
{
    if (isActive_)
    {
        Vector2I mouseTile{UnitConversion::screenToTile(GetMousePosition(), camera)};

        if (!(position_.tilePosition() == mouseTile))
        {
            position_.changeTo(mouseTile);

            snx::PublisherStatic::publish(Event::cursorPositionChanged);
        }

        return;
    }

    position_.changeTo(heroPosition);
}
