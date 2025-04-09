#include "Tiles.h"

#include "Convert.h"
#include "DenseMap.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <algorithm>
#include <unordered_set>

namespace TilesModule
{
    Tiles const& insert(
        Tiles& tiles,
        Vector2I const& tilePosition,
        RenderId renderId,
        bool isSolid,
        bool isOpaque,
        VisibilityId visibilityId
    )
    {
        //* Check if id exists to avoid usage of new id for existing tilePosition
        size_t id = ( tiles.ids.contains( tilePosition ) )
                        ? tiles.ids.at( tilePosition )
                        : Tiles::idManager.requestId();

        tiles.ids.insert_or_assign( tilePosition, id );

        tiles.positions.insert_or_assign( id, Convert::tileToWorld( tilePosition ) );

        tiles.renderIds.insert_or_assign( id, renderId );

        tiles.visibilityIds.insert_or_assign( id, visibilityId );

        if ( isSolid )
        {
            tiles.isSolids.insert( id );
        }

        else
        {
            tiles.isSolids.erase( id );
        }

        if ( isOpaque )
        {
            tiles.isOpaques.insert( id );
        }

        else
        {
            tiles.isOpaques.erase( id );
        }

        tiles.mapSize = RectangleExI{
            Vector2I{
                std::min( tilePosition.x, tiles.mapSize.left() ),
                std::min( tilePosition.y, tiles.mapSize.top() )
            },
            Vector2I{
                std::max( tilePosition.x, tiles.mapSize.right() ),
                std::max( tilePosition.y, tiles.mapSize.bottom() )
            }
        };

        assert( tiles.ids.size() == tiles.positions.size() && "Tiles.ids mismatches Tiles.positions" );

        return tiles;
    }
}
