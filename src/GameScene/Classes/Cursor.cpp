#include "Cursor.h"

#include "UnitConversion.h"
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
        position_.changeTo(UnitConversion::getMouseTile(camera));
        return;
    }

    position_.changeTo(heroPosition);
}