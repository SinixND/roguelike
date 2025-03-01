#include "Tiles.h"

#include "Convert.h"
#include "DenseMap.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <algorithm>
#include <unordered_set>

void Tiles::insert(
    Vector2I const& tilePosition,
    RenderId renderId,
    bool isSolid,
    bool isOpaque,
    VisibilityId visibilityId
)
{
    //* Check if id exists to avoid usage of new id for existing tilePosition
    size_t id = ( ids.contains( tilePosition ) )
                    ? ids.at( tilePosition )
                    : idManager_.requestId();

    ids.insert_or_assign( tilePosition, id );

    positions.insert_or_assign( id, Convert::tileToWorld( tilePosition ) );

    renderIds.insert_or_assign( id, renderId );

    visibilityIds.insert_or_assign( id, visibilityId );

    if ( isSolid )
    {
        isSolids.insert( id );
    }

    else
    {
        isSolids.erase( id );
    }

    if ( isOpaque )
    {
        isOpaques.insert( id );
    }

    else
    {
        isOpaques.erase( id );
    }

    mapSize = RectangleExI{
        Vector2I{
            std::min( tilePosition.x, mapSize.left() ),
            std::min( tilePosition.y, mapSize.top() )
        },
        Vector2I{
            std::max( tilePosition.x, mapSize.right() ),
            std::max( tilePosition.y, mapSize.bottom() )
        }
    };

    assert( ids.size() == positions.size() && "Tiles.ids mismatches Tiles.positions" );
}
