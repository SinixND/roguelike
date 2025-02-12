#include "Cursor.h"

#include "Convert.h"
#include "EventId.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>

namespace CursorModule
{
    [[nodiscard]]
    Cursor const& toggle( Cursor& cursor )
    {
        cursor.isActive = !cursor.isActive;

        if ( cursor.isActive )
        {
            HideCursor();
            return cursor;
        }

        ShowCursor();

        return cursor;
    }

    [[nodiscard]]
    Cursor const& update(
        Cursor& cursor,
        Camera2D const& camera,
        Vector2I const& heroPosition
    )
    {
        //* Update cursor position if active
        if ( cursor.isActive )
        {
            Vector2I mouseTile{ Convert::screenToTile( GetMousePosition(), camera ) };

            if ( !( Convert::worldToTile( cursor.position ) == mouseTile ) )
            {
                cursor.position = Convert::tileToWorld( mouseTile );

                snx::PublisherStatic::publish( EventId::CURSOR_POSITION_CHANGED );
            }

            return cursor;
        }

        //* Keep cursor on hero if inactive
        cursor.position = Convert::tileToWorld( heroPosition );

        return cursor;
    }
}
