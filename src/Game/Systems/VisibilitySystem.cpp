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

/// Horizontal shadow projection line, defined by left and right slope,
/// get extended by opaque tiles while projected away from viewpoint (hero).
/// Origin for slope is the viewpoint tile corner
/// opposite to expansion direction, not the tiles center!
/// See https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/
class ShadowLine
{
public:
    float slopeLeft{};
    float slopeRight{};

public:
    explicit ShadowLine( Vector2I const& octantPosition )
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
VisibilityId getVisibilityOutOfVision( VisibilityId tileVisibilityOld )
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

enum NewSlopesCovered
{
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    BOTH = 3,
};

/// Which newShadow slope is covered by the old shadow
NewSlopesCovered getSlopesCovered(
    ShadowLine const& oldShadowLine,
    ShadowLine const& newShadowLine
)
{
    //* NOTE: Slope values get bigger CCW!
    bool isLeftCovered{
        oldShadowLine.slopeLeft >= newShadowLine.slopeLeft
        && newShadowLine.slopeLeft >= oldShadowLine.slopeRight
    };

    bool isRightCovered{
        oldShadowLine.slopeLeft >= newShadowLine.slopeRight
        && newShadowLine.slopeRight >= oldShadowLine.slopeRight
    };

    return static_cast<NewSlopesCovered>( isLeftCovered ^ ( isRightCovered << 1 ) );
}

[[nodiscard]]
std::vector<ShadowLine>& mergeNewShadow(
    std::vector<ShadowLine>& shadows,
    Vector2I const& octantPosition
)
{
    ShadowLine newShadow{ octantPosition };

    //* Merge newShadow with existing shadows
    auto itr{ shadows.begin() };
    // for ( size_t idx{ 0 }; idx < shadowsSize; ++idx )
    while ( itr != shadows.end() )
    {
        ShadowLine& oldShadow{ *itr };

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

[[nodiscard]]
Tiles const& calculateVisibilitiesInOctant(
    Tiles& tiles,
    snx::DenseMap<Vector2I, Fog>& fogsIO,
    Vector2I const& heroPosition,
    int octant,
    int visionRange,
    int range
)
{
    //* NOTE: Octant coordinates are standard cartesian tilePositions (centered)
    //* x+ = right, y+ = up !!!
    std::vector<ShadowLine> shadows{};

    //* Iterate octant
    //* Octant[0] is from vertical up CW,
    //* Octants enumerated CW
    for ( int octY{ 0 }; octY < range; ++octY )
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

            //* Check existence
            if ( !tiles.ids.contains( tilePosition ) )
            {
                continue;
            }

            size_t tileId{ tiles.ids.at( tilePosition ) };

            VisibilityId tileVisibilityOld{ tiles.visibilityIds.at( tileId ) };

            //* < : Update only octant tiles including diagonal tiles (spare last row tile, needed for correct diagonal visibility)
            if ( octX <= octY )
            {
                //* Skip test (. set invis) if shadowline already covers whole octant
                if ( shadows.size()
                     && shadows[0].slopeLeft < 0
                     && shadows[0].slopeRight < 1 )
                {
                    tiles.visibilityIds.at( tileId ) = getVisibilityOutOfVision( tileVisibilityOld );
                    fogsIO.insert_or_assign( tilePosition ) = getFogOfWarState( tileVisibilityOld );

                    continue;
                } //* Max shadow

                //* Check visibility by intersection with shadow line elements
                bool isVisible{ true };

                //* Check if tile is out of range (viewport and visionRange)
                if ( !(
                         ( octY < ( range - 1 ) )
                         && sqrt( pow( octX, 2 ) + pow( octY, 2 ) ) < visionRange
                     ) )
                {
                    isVisible = false;
                }
                else
                {
                    for ( ShadowLine const& oldShadow : shadows )
                    {
                        //* Check if visible:
                        //* If one shadow covers tile -> invis
                        ShadowLine testShadow{ { octX, octY } };

                        if ( getSlopesCovered( oldShadow, testShadow ) == BOTH )
                        {
                            isVisible = false;

                            break;
                        }
                    }
                }

                //* Update visibility
                if ( isVisible )
                {
                    //* Tile IS visible
                    tiles.visibilityIds.at( tileId ) = VisibilityId::VISIBILE;
                }

                else
                {
                    tiles.visibilityIds.at( tileId ) = getVisibilityOutOfVision( tileVisibilityOld );
                    fogsIO.insert_or_assign( tilePosition ) = getFogOfWarState( tileVisibilityOld );
                }
            } //* Octant tiles only

            //* Update shadow line
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
    [[nodiscard]]
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
        tiles.visibilityIds.at( tiles.ids.at( heroPosition ) ) = VisibilityId::VISIBILE;

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
