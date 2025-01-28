#include "PathfinderSystem.h"

//* #define DEBUG_PATHFINDER

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

//* RatedTile
//* Heuristic used to rate tiles
//* bias > 1: prioritize short path
//* bias < 1: prioritize closer to target
int constexpr bias{ 2 };

struct RatedTile
{
    Vector2I tilePosition{};
    Vector2I distanceToTarget{};
    int stepsNeeded{};
    RatedTile* ancestor{ nullptr };

    RatedTile(
        Vector2I const& tilePosition,
        Vector2I const& target,
        int stepsNeeded,
        RatedTile* ancestor = nullptr
    )
        : tilePosition( tilePosition )
        , distanceToTarget( Vector2Subtract( target, tilePosition ) )
        , stepsNeeded( stepsNeeded )
        , ancestor( ancestor )
    {
    }
};

namespace RatedTileModule
{
    //* Heuristic used to rate tiles
    int getRating( RatedTile const& ratedTile )
    {
        return
            //* Distance to target
            Vector2Length( ratedTile.distanceToTarget )
            + bias * ratedTile.stepsNeeded;
    }

    void reconstructPath(
        RatedTile const& ratedTile,
        std::vector<Vector2I>& path
    )
    {
        //* Add this to path
        path.push_back( ratedTile.tilePosition );

        //* Abort (includes root/start)
        if ( !ratedTile.ancestor )
        {
            return;
        }

        //* Add ancestor to path
        reconstructPath(
            *ratedTile.ancestor,
            path
        );
    }

}

//* Adds three new neighbours for all tiles of current best rating to the rating list,
//* then deleting current rating in map
//* and restarting with new best rating,
//* until target is found or no rating left to check
bool checkRatingList(
    int currentRating,
    std::forward_list<RatedTile>& ratedTiles,
    std::map<int, std::vector<RatedTile*>>& ratingList,
    std::unordered_set<Vector2I>& tilesToIgnore,
    Map const& map,
    Vector2I const& target,
    GameCamera const& gameCamera,
    int maxRange,
    std::vector<Vector2I>& path
)
{
    //* Buffer rated tiles to allow neighbours with same rating
    std::vector<RatedTile*> tileList{ ratingList[currentRating] };

    //* All tiles with same rating will be checked . remove key
    ratingList.erase( currentRating );

    //* Check all tiles in vector for current best rating before choosing new best rating
    for ( RatedTile* currentTile : tileList )
    {
        Vector2I distanceToTarget{ currentTile->distanceToTarget };
        Vector2I mainDirection{ Vector2MainDirection( distanceToTarget ) };
        Vector2I offDirection{ Vector2OffDirection( distanceToTarget ) };

        //* Handle exceptions
        //* Exception: |x| == |y| . main is RNG, off is dependent
        if ( abs( distanceToTarget.x ) == abs( distanceToTarget.y ) )
        {
            if ( snx::RNG::random() )
            {
                mainDirection = Vector2Normalize(
                    Vector2I{
                        distanceToTarget.x,
                        0
                    }
                );

                offDirection = Vector2Normalize(
                    Vector2I{
                        0,
                        distanceToTarget.y
                    }
                );
            }
            else
            {
                mainDirection = Vector2Normalize(
                    Vector2I{
                        0,
                        distanceToTarget.y
                    }
                );

                offDirection = Vector2Normalize(
                    Vector2I{
                        distanceToTarget.x,
                        0
                    }
                );
            }
        }

        //* Exception: offDirection == {0 , 0}
        if ( offDirection == Vector2I{ 0, 0 } )
        {
            if ( snx::RNG::random() )
            {
                offDirection = Vector2Swap( mainDirection );
            }
            else
            {
                offDirection = Vector2Negate( Vector2Swap( mainDirection ) );
            }
        }

        //* Test all four directions for currentTile, prioritise main direction to target
        for ( Vector2I const& direction : {
                  mainDirection,
                  offDirection,
                  Vector2Negate( offDirection ),
                  Vector2Negate( mainDirection )
              } )
        {
            //* Calculate new tilePosition
            Vector2I newTilePosition{
                Vector2Add(
                    direction,
                    currentTile->tilePosition
                )
            };

            //* Needs to be in viewport
            if ( !CheckCollisionPointRec(
                     Convert::tileToScreen(
                         newTilePosition,
                         gameCamera.camera
                     ),
                     *gameCamera.viewport
                 ) )
            {
                continue;
            }

            //* Ignore ancestor
            if ( currentTile->ancestor
                 && ( newTilePosition == currentTile->ancestor->tilePosition ) )
            {
                continue;
            }

            //* Add rating penalty if
            //* - Enemy is present
            int penalty{ 0 };

            if ( map.enemies.ids.contains( newTilePosition ) )
            {
                penalty += 4;
            }

            RatedTile newRatedTile{
                newTilePosition,
                target,
                currentTile->stepsNeeded + ( 1 + penalty ),
                currentTile
            };

            //* If Target found
            if ( ( newTilePosition == target ) )
            {
                RatedTileModule::reconstructPath(
                    newRatedTile,
                    path
                );

                return true;
            }

            //* Skip if is in tilesToIgnore
            if ( tilesToIgnore.contains( newTilePosition ) )
            {
                continue;
            }

            //* Skip if tile is invalid:
            //* - Not in map
            //* - Is invisible
            //* - Not accessible
            //* - Steps needed exceed maxRange
            if ( !map.tiles.ids.contains( newTilePosition ) )
            {
                return false;
            }

            size_t tileId{ map.tiles.ids.at( newTilePosition ) };

            if ( !map.tiles.visibilityIds.contains( tileId )
                 || ( map.tiles.visibilityIds.at( tileId ) == VisibilityId::INVISIBLE )
                 || map.tiles.isSolids.contains( tileId )
                 || ( ( maxRange > 0 ) && ( newRatedTile.stepsNeeded > maxRange ) ) )
            {
                //* Invalid! Add to ignore set so it doesn't get checked again
                tilesToIgnore.insert( newTilePosition );

                continue;
            }

            //* Create new ratedTile in vector
            ratedTiles.push_front( newRatedTile );

            //* Add newRatedTile
            ratingList[RatedTileModule::getRating( newRatedTile )].push_back( &ratedTiles.front() );

            //* Valid! Add to ignore set so it doesn't get checked again
            tilesToIgnore.insert( newTilePosition );

#if defined( DEBUG ) && defined( DEBUG_PATHFINDER )
            DrawText(
                std::format(
                    "{:.0f}",
                    newRatedTile.rating()
                )
                    .c_str(),
                Convert::tileToScreen(
                    newTilePosition,
                    snx::debug::gcam().camera()
                )
                        .x
                    + 10,
                Convert::tileToScreen(
                    newTilePosition,
                    snx::debug::gcam().camera()
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
    if ( !ratingList.empty()
         && checkRatingList(
             ratingList.begin()->first,
             ratedTiles,
             ratingList,
             tilesToIgnore,
             map,
             target,
             gameCamera,
             maxRange,
             path
         ) )
    {
        return true;
    }

    return false;
}

namespace PathfinderSystem
{
    std::vector<Vector2I> findPath(
        Map const& map,
        Vector2I const& start,
        Vector2I const& target,
        GameCamera const& gameCamera,
        bool skipInvisibleTiles,
        int maxRange
    )
    {
#if defined( DEBUG ) && defined( DEBUG_PATHFINDER )
        BeginDrawing();
#endif

        std::vector<Vector2I> path{};

        //* Return empty path if target is
        //* - Not in map
        //* - Is invisible
        //* - Not accessible
        //* - Equal to start
        if ( !map.tiles.ids.contains( target ) )
        {
            return path;
        }

        size_t tileId{ map.tiles.ids.at( target ) };

        if ( !map.tiles.visibilityIds.contains( tileId )
             || ( skipInvisibleTiles
                  && ( map.tiles.visibilityIds.at( tileId ) == VisibilityId::INVISIBLE ) )
             || map.tiles.isSolids.contains( tileId )
             || ( start == target ) )
        {
            return path;
        }

        //* Create tile to start
        RatedTile firstTile{
            start,
            target,
            0,
            nullptr
        };

        //* Vector of rated tiles (persistent for ancestor pointers)
        std::forward_list<RatedTile> ratedTiles{ firstTile };

        //* Map of tile pointers, sorted by rating (lowest first)
        std::map<int, std::vector<RatedTile*>> ratingList{};

        ratingList[RatedTileModule::getRating( firstTile )].push_back( &ratedTiles.front() );

        //* List of ignored tiles to avoid double checks
        std::unordered_set<Vector2I> tilesToIgnore{ start };

        checkRatingList(
            RatedTileModule::getRating( firstTile ),
            ratedTiles,
            ratingList,
            tilesToIgnore,
            map,
            target,
            gameCamera,
            maxRange,
            path
        );

        //* Path is either empty or has at least 2 entries (target and start)
#if defined( DEBUG ) && defined( DEBUG_PATHFINDER )
        for ( Vector2I const& position : path )
        {
            DrawCircleV(
                Vector2Add(
                    Convert::tileToScreen(
                        position,
                        snx::debug::gcam().camera()
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
