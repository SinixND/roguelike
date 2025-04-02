#include "VisibilitySystem.h"

#include "Convert.h"
#include "DenseMap.h"
#include "Fog.h"
#include "Tiles.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <unordered_set>
#include <vector>

/// Shadow sector, defined by left and right slope,
/// get extended by opaque tiles while projected away from viewpoint (hero).
/// Origin for slope is the viewpoint tile corner
/// opposite to expansion direction, not the tiles center!
/// See https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/
class Shadow
{
public:
    float slopeLeft{};
    float slopeRight{};

public:
    explicit Shadow( Vector2I const& octantPosition )
    {
        assert( ( octantPosition.x || octantPosition.y ) && "Cant compute slope for origin" );

        setSlopeLeft( octantPosition );

        setSlopeRight( octantPosition );
    }

    void setSlopeLeft( Vector2I const& octantPosition )
    {
        //* Keep left slope from flipping to negative values
        slopeLeft =
            ( ( ( octantPosition.y + 0.5f ) / ( octantPosition.x - 0.5f ) )
              * /* prevent negative */ !!octantPosition.x )
            + /* keep > 0 */ ( std::numeric_limits<float>::max() * !octantPosition.x );
    }

    void setSlopeRight( Vector2I const& octantPosition )
    {
        slopeRight = ( octantPosition.y - 0.5f ) / ( octantPosition.x + 0.5f );
    }
};

//* VisibilitySystem
VisibilityId getOutOfVisionVisibility( VisibilityId tileVisibilityOld )
{
    VisibilityId visibilityId{ tileVisibilityOld };

    if ( tileVisibilityOld == VisibilityId::VISIBILE )
    {
        //* Tile WAS visible
        visibilityId = VisibilityId::SEEN;
    }

    return visibilityId;
}

Fog getFogOfWarState( VisibilityId tileVisibility )
{
    Fog fog{};

    if ( tileVisibility == VisibilityId::VISIBILE
         || tileVisibility == VisibilityId::SEEN )
    {
        fog = Fog::TRANSPARENT;
    }

    else
    {
        fog = Fog::OPAQUE;
    }

    return fog;
}

enum SlopesCovered
{
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    BOTH = 3,
};

/// Which newShadow slope is covered by the old shadow
SlopesCovered getSlopesCovered(
    Shadow const& oldShadow,
    Shadow const& newShadow
)
{
    //* NOTE: Slope values get bigger CCW!
    bool isLeftCovered{
        oldShadow.slopeLeft >= newShadow.slopeLeft
        && newShadow.slopeLeft >= oldShadow.slopeRight
    };

    bool isRightCovered{
        oldShadow.slopeLeft >= newShadow.slopeRight
        && newShadow.slopeRight >= oldShadow.slopeRight
    };

    return static_cast<SlopesCovered>( isLeftCovered ^ ( isRightCovered << 1 ) );
}

[[nodiscard]]
std::vector<Shadow>& mergeNewShadow(
    std::vector<Shadow>& shadows,
    Vector2I const& octantPosition
)
{
    Shadow newShadow{ octantPosition };

    //* Merge newShadow with existing shadows
    auto itr{ shadows.begin() };
    while ( itr != shadows.end() )
    {
        Shadow const& oldShadow{ *itr };

        switch ( getSlopesCovered(
            oldShadow,
            newShadow
        ) )
        {
            default:
            case NONE:
            {
                ++itr;
                break;
            }

            case BOTH:
            {
                return shadows;
            }

            case RIGHT:
            {
                newShadow.slopeRight = oldShadow.slopeRight;

                shadows.erase( itr );

                break;
            }

            case LEFT:
            {
                newShadow.slopeLeft = oldShadow.slopeLeft;

                shadows.erase( itr );

                break;
            }
        }
    }

    shadows.push_back( newShadow );

    return shadows;
}

bool checkVisibility(
    std::vector<Shadow> const& shadows,
    int visionRange,
    int octX,
    int octY
)
{
    //* Check if tile is out of range (viewport and visionRange)
    if ( !( visionRange > sqrt( pow( octX, 2 ) + pow( octY, 2 ) ) ) )
    {
        return false;
    }

    //* Compare tile against all existing shadows
    for ( Shadow const& oldShadow : shadows )
    {
        //* Compare if tile is covered in existing shadow
        Shadow testShadow{ { octX, octY } };

        if ( getSlopesCovered( oldShadow, testShadow ) == BOTH )
        {
            return false;
        }
    }

    return true;
}

[[nodiscard]]
VisibilityId& updateVisibilityId(
    VisibilityId& visibilityId,
    snx::DenseMap<Vector2I, Fog>& fogsIO,
    std::vector<Shadow> const& shadows,
    Vector2I const& tilePosition,
    int visionRange,
    Vector2I const& octantPosition
)
{
    VisibilityId tileVisibilityOld{ visibilityId };

    if ( checkVisibility(
             shadows,
             visionRange,
             octantPosition.x,
             octantPosition.y
         ) )
    {
        //* Tile IS visible
        visibilityId = VisibilityId::VISIBILE;
    }
    else
    {
        visibilityId = getOutOfVisionVisibility( tileVisibilityOld );
        fogsIO.insert_or_assign( tilePosition ) = getFogOfWarState( tileVisibilityOld );
    }

    return visibilityId;
}

[[nodiscard]]
Tiles const& calculateVisibilitiesInOctant(
    Tiles& tiles,
    snx::DenseMap<Vector2I, Fog>& fogsIO,
    Vector2I const& heroPosition,
    int octant,
    int visionRange,
    int viewportRange
)
{
    //* NOTE: Octant coordinates are standard cartesian tilePositions (centered)
    //* x+ = right, y+ = up !!!
    std::vector<Shadow> shadows{};

    //* Iterate octant rows until diagonal + 1!
    //* Octant[0] is from vertical up CW,
    //* Octants enumerated CW
    for ( int octY{ 0 }; octY < viewportRange; ++octY )
    {
        for ( int octX{ 0 }; octX <= octY + 1; ++octX )
        {
            Vector2I octantPosition{ octX, octY };

            Vector2I tilePosition{
                Convert::octantToTile(
                    octantPosition,
                    octant,
                    heroPosition
                )
            };

            //* Ensure tile existence
            if ( !tiles.ids.contains( tilePosition ) )
            {
                continue;
            }

            size_t tileId{ tiles.ids.at( tilePosition ) };

            //* Update only octant tiles up to and including diagonal tiles
            if ( octX <= octY )
            {
                tiles.visibilityIds.at( tileId ) = updateVisibilityId(
                    tiles.visibilityIds.at( tileId ),
                    fogsIO,
                    shadows,
                    tilePosition,
                    visionRange,
                    octantPosition
                );
            }

            //* Update shadows for all iterated tiles
            if ( !tiles.isOpaques.contains( tileId ) )
            {
                continue;
            }

            shadows = mergeNewShadow(
                shadows,
                octantPosition
            );
        } //* Octant column
    } //* Octant row

    return tiles;
}

namespace VisibilitySystem
{
    Tiles const& calculateVisibilities(
        Tiles& tiles,
        snx::DenseMap<Vector2I, Fog>& fogsIO,
        RectangleExI const& viewportInTiles,
        Vector2I const& heroPosition,
        int visionRange
    )
    {
        //* Input
        int quarterWidth{ 2 + ( viewportInTiles.width() / 2 ) };
        int quarterHeight{ 2 + ( viewportInTiles.height() / 2 ) };

        //* Init
        fogsIO.clear();

        //* Hero is always visible
        tiles.visibilityIds[tiles.ids.index( heroPosition )] = VisibilityId::VISIBILE;

        //* Iterate octants:
        //* Orientation dependent range (horizontal, vertical)
        int range{};

        //* Octant[0] is from vertical up CW,
        //* Octants enumerated CW
        for ( int octant{ 0 }; octant < 8; ++octant )
        {
            //* Set range for octant
            //* viewport dependent:
            if ( ( ( octant + 1 ) / 2 ) % 2 ) //* := f tt ff tt f
            {
                range = quarterWidth;
            }
            else
            {
                range = quarterHeight;
            }

            tiles = calculateVisibilitiesInOctant(
                tiles,
                fogsIO,
                heroPosition,
                octant,
                visionRange,
                range
            );
        }

        return tiles;
    }
}
