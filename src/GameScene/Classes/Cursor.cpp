#include "Cursor.h"

#include "Event.h"
#include "PublisherStatic.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

void Cursor::toggle()
{
    isActive_ = !isActive_;

    if (isActive_)
    {
        ShowCursor();
        return;
    }

    HideCursor();
}

void Cursor::update(Camera2D const& camera, Vector2 const& heroPosition)
{
    if (isActive_)
    {
        if (!Vector2Equals(position_.tilePosition(), UnitConversion::getMouseTile(camera)))
        {
            position_.changeTo(UnitConversion::getMouseTile(camera));

            snx::PublisherStatic::publish(Event::cursorPositionChanged);
        }

        return;
    }

    position_.changeTo(heroPosition);
}