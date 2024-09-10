#include "Cursor.h"

#include "Event.h"
#include "Position.h"
#include "PublisherStatic.h"
#include "RenderID.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

Position& Cursor::position() { return position_; }

RenderID Cursor::renderID() { return renderID_; }

bool Cursor::isActive() { return isActive_; }

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