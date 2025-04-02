#include "PathfinderSystem.h"

// #define DEBUG_PATHFINDER

#include "Convert.h"
#include "GameCamera.h"
#include "Map.h"
#include "RNG.h"
#include "Tiles.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <cstdlib>
#include <forward_list>
#include <map>
#include <raylib.h>
#include <raymath.h>
#include <unordered_set>
#include <vector>

#if defined( DEBUG ) && defined( DEBUG_PATHFINDER )
#include "Debugger.h"
#include "TileData.h"
#include <format>
#include <string>
#endif

/// Used for heuristic to rate tiles:
/// bias > 1: prioritize short path,
/// bias < 1: prioritize direct path
int constexpr BIAS{ 2 };

struct AStarTile
{
    Vector2I tilePosition{};
    Vector2I offsetToTarget{};
    AStarTile* ancestor{ nullptr };
    int stepsNeeded{};
    int penalty{ 0 };

    AStarTile(
        Vector2I const& tilePosition,
        Vector2I const& target,
        int stepsNeeded,
        AStarTile* ancestor = nullptr
    )
        : tilePosition( tilePosition )
        , offsetToTarget( Vector2Subtract( target, tilePosition ) )
        , ancestor( ancestor )
        , stepsNeeded( stepsNeeded )
    {
    }
};

namespace AStarTileModule
{
    //* Heuristic used to rate tiles
    int getRating( AStarTile const& ratedTile )
    {
        return
            //* Distance to target
            Vector2Length( ratedTile.offsetToTarget )
            + BIAS * ratedTile.stepsNeeded;
    }

    void reconstructPath(
        std::vector<Vector2I>& path,
        AStarTile const& aStarTile
    )
    {
        //* Add this to path
        path.push_back( aStarTile.tilePosition );

        //* Abort (includes root/start)
        if ( !aStarTile.ancestor )
        {
            return;
        }

        //* Add ancestor to path
        reconstructPath(
            path,
            *aStarTile.ancestor
        );
    }

}

/// Check if target is
/// - in map
/// - accessible aka. not solid
/// - visible
/// - not equal to start
bool isTileValid(
    Vector2I const& targetToCheck,
    Vector2I const& start,
    Tiles const& tiles,
    bool skipInvisibleTiles
)
{
    if ( !tiles.ids.contains( targetToCheck ) )
    {
        return false;
    }

    size_t tileId{ tiles.ids.at( targetToCheck ) };

    return !(
        tiles.isSolids.contains( tileId )
        || ( skipInvisibleTiles
             && ( tiles.visibilityIds.at( tileId ) == VisibilityId::INVISIBLE )
        )
        || ( start == targetToCheck )
    );
}

/// Check if target is
/// - in map
/// - visible
/// - accessible
/// - Steps needed within maxRange
bool isTileValid(
    AStarTile const& targetToCheck,
    Tiles const& tiles,
    int maxRange
)
{
    if ( !tiles.ids.contains( targetToCheck.tilePosition ) )
    {
        return false;
    }

    size_t tileId{ tiles.ids.at( targetToCheck.tilePosition ) };

    return !(
        tiles.isSolids.contains( tileId )
        || ( tiles.visibilityIds.at( tileId ) == VisibilityId::INVISIBLE )
        || ( ( maxRange > 0 ) && ( targetToCheck.stepsNeeded > maxRange ) )
    );
}

/// Get main- and off direction while using RNG
/// for horizontal, vertical and diagnoal offset
struct GuidedDirectionRNG
{
    Vector2I main;
    Vector2I off;

    explicit GuidedDirectionRNG( Vector2I const& offset )
        : main( Vector2MainDirection( offset ) )
        , off( Vector2OffDirection( offset ) )
    {
        //* Handle exceptions
        //* Exception: |x| == |y| => main is RNG, off is dependent
        if ( abs( offset.x ) == abs( offset.y ) )
        {
            if ( snx::RNG::random() )
            {
                main = Vector2Normalize(
                    Vector2I{
                        offset.x,
                        0
                    }
                );

                off = Vector2Normalize(
                    Vector2I{
                        0,
                        offset.y
                    }
                );
            }
            else
            {
                main = Vector2Normalize(
                    Vector2I{
                        0,
                        offset.y
                    }
                );

                off = Vector2Normalize(
                    Vector2I{
                        offset.x,
                        0
                    }
                );
            }
        }

        //* Exception: guidedDirection.off == {0 , 0}
        if ( off == Vector2I{ 0, 0 } )
        {
            if ( snx::RNG::random() )
            {
                off = Vector2Swap( main );
            }
            else
            {
                off = Vector2Negate( Vector2Swap( main ) );
            }
        }
    }
};

/// For all tiles with the best rating in sortedTiles,
/// add the three new/next neighbours to the sortedTiles,
/// and recursively restart with newly determined best rating.
/// Returns true if target is found or no rating left to check
bool continueTargetSearch(
    std::vector<Vector2I>& path,
    std::forward_list<AStarTile>& tileStorage,
    std::map<int, std::vector<AStarTile*>>& sortedTiles,
    std::unordered_set<Vector2I>& tilesToIgnore,
    Map const& map,
    Vector2I const& target,
    GameCamera const& gameCamera,
    int currentRating,
    int maxRange
)
{
    //* Buffer AStarTiles to process and delete map entry for currentRating
    //* to allow neighbours with similar rating
    std::vector<AStarTile*> tileBuffer{ sortedTiles[currentRating] };

    sortedTiles.erase( currentRating );

    //* Check all tiles in vector for current best rating before choosing new best rating
    for ( AStarTile* currentTile : tileBuffer )
    {
        GuidedDirectionRNG guidedDirection{ currentTile->offsetToTarget };

        //* Check all four directions for currentTile, prioritise main direction to target
        for ( Vector2I const& direction : {
                  guidedDirection.main,
                  guidedDirection.off,
                  Vector2Negate( guidedDirection.off ),
                  Vector2Negate( guidedDirection.main )
              } )
        {
            //* Calculate new tilePosition to check
            AStarTile newAStarTile{
                Vector2Add(
                    direction,
                    currentTile->tilePosition
                ),
                target,
                currentTile->stepsNeeded + 1,
                currentTile
            };

            //* If Target found
            if ( ( newAStarTile.tilePosition == target ) )
            {
                AStarTileModule::reconstructPath(
                    path,
                    newAStarTile
                );

                return true;
            }

            //* Skip if
            if (
                //* Ignore ancestor
                ( currentTile->ancestor
                  && ( newAStarTile.tilePosition == currentTile->ancestor->tilePosition ) )
                || !isTileValid(
                    newAStarTile,
                    map.tiles,
                    maxRange
                )
                //* Needs to be in viewport
                || !CheckCollisionPointRec(
                    Convert::tileToScreen(
                        newAStarTile.tilePosition,
                        gameCamera.camera
                    ),
                    *gameCamera.viewport
                )
            )
            {
                continue;
            }

            //* Skip and ignore
            if ( tilesToIgnore.contains( newAStarTile.tilePosition ) )
            {
                tilesToIgnore.insert( newAStarTile.tilePosition );
                continue;
            }

            //* Add rating penalty if
            //* - enemy would block path
            if ( map.enemies.ids.contains( newAStarTile.tilePosition ) )
            {
                //* Penalty = 4 is equal to the direct path around the enemy
                newAStarTile.penalty += 4;
            }

            //* Create and sort newRatedTile
            tileStorage.push_front( newAStarTile );

            sortedTiles[AStarTileModule::getRating( newAStarTile )].push_back( &tileStorage.front() );

            //* Valid! Add to ignore set so it doesn't get checked again
            tilesToIgnore.insert( newAStarTile.tilePosition );

#if defined( DEBUG ) && defined( DEBUG_PATHFINDER )
            DrawText(
                std::format(
                    "{:.0f}",
                    newAStarTile.rating()
                )
                    .c_str(),
                Convert::tileToScreen(
                    newTilePosition,
                    snx::Debugger::gcam().camera()
                )
                        .x
                    + 10,
                Convert::tileToScreen(
                    newTilePosition,
                    snx::Debugger::gcam().camera()
                )
                        .y
                    + 10,
                10,
                WHITE
            );
#endif
        }
    }

    //* Check new best rated tiles
    if ( sortedTiles.empty()
         || continueTargetSearch(
             path,
             tileStorage,
             sortedTiles,
             tilesToIgnore,
             map,
             target,
             gameCamera,
             sortedTiles.begin()->first,
             maxRange
         ) )
    {
        return true;
    }

    return false;
}

namespace PathfinderSystem
{
    std::vector<Vector2I> calculateAStarPath(
        Vector2I const& start,
        Vector2I const& target,
        Map const& map,
        GameCamera const& gameCamera,
        bool skipInvisibleTiles,
        int maxRange
    )
    {
#if defined( DEBUG ) && defined( DEBUG_PATHFINDER )
        BeginDrawing();
#endif

        std::vector<Vector2I> path{};

        //* Return empty path if target is invalid
        if ( !isTileValid(
                 target,
                 start,
                 map.tiles,
                 skipInvisibleTiles
             ) )
        {
            return path;
        };

        //* Create tile to start path from
        AStarTile firstTile{
            start,
            target,
            0,
            nullptr
        };

        //* AStarTiles storage to reconstruct path from
        //* Persistent/non-dynamic storage needed to keep ancestor pointer valid
        std::forward_list<AStarTile> tileStorage{ firstTile };

        //* Map of AStarTile pointers to aStarTileStorage
        //* Sorted by rating (lowest rating = best)
        std::map<int, std::vector<AStarTile*>> sortedTiles{};

        sortedTiles[AStarTileModule::getRating( firstTile )]
            .push_back( &tileStorage.front() );

        //* List of ignored tiles to avoid double checks with worse rating
        std::unordered_set<Vector2I> tilesToIgnore{ start };

        continueTargetSearch(
            path,
            tileStorage,
            sortedTiles,
            tilesToIgnore,
            map,
            target,
            gameCamera,
            AStarTileModule::getRating( firstTile ),
            maxRange
        );

        //* Path is either empty or has at least 2 entries (target and start)
#if defined( DEBUG ) && defined( DEBUG_PATHFINDER )
        for ( Vector2I const& position : path )
        {
            DrawCircleV(
                Vector2Add(
                    Convert::tileToScreen(
                        position,
                        snx::Debugger::gcam().camera()
                    ),
                    Vector2{ TileData::TILE_SIZE_HALF, TileData::TILE_SIZE_HALF }
                ),
                5,
                ColorAlpha(
                    GREEN,
                    0.5
                )
            );
        }

        EndDrawing();
#endif

        return path;
    }
}
