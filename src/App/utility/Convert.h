#ifndef IG20240215012142
#define IG20240215012142

#include "ChunkData.h"
#include "TileData.h"
#include "raylibEx.h"
#include <cmath>
#include <raylib.h>
#include <raymath.h>

//* This file contains functions to convert between
//* - tile (int/position, game codrdinates)
//* - world (float/pixel, game coordinates)
//* - chunk (int/position, game coordinates)
//* Camera transforms between game and screen coordinates
//* - screen (float/pixel, screen coordinates)
namespace Convert
{
    //* World pixel to tile position
    inline Vector2I worldToTile( Vector2 const& pixel )
    {
        //* World pixel is center of tile
        return Vector2I{
            static_cast<int>( std::floor( ( pixel.x ) / TileData::TILE_SIZE ) ),
            static_cast<int>( std::floor( ( pixel.y ) / TileData::TILE_SIZE ) )
        };
    }

    //* Tile position to world pixel
    inline Vector2 tileToWorld( Vector2I const& tilePosition )
    {
        return Vector2{
            ( tilePosition.x * TileData::TILE_SIZE ),
            ( tilePosition.y * TileData::TILE_SIZE )
        };
    }

    //* Screen pixel to world pixel to tile position
    inline Vector2I screenToTile(
        Vector2 const& pixel,
        Camera2D const& camera
    )
    {
        Vector2 worldPixel{ GetScreenToWorld2D( pixel, camera ) };

        return worldToTile( worldPixel );
    }

    //* Tile position to world pixel to screen pixel
    inline Vector2 tileToScreen(
        Vector2I const& tilePosition,
        Camera2D const& camera
    )
    {
        Vector2 worldPixel{ tileToWorld( tilePosition ) };

        return GetWorldToScreen2D( worldPixel, camera );
    }

    inline Vector2I getMouseTile( Camera2D const& camera )
    {
        return screenToTile(
            GetMousePosition(),
            camera
        );
    }

    //* Tile position to chunk position
    inline Vector2I tileToChunk( Vector2I const& tilePosition )
    {
        return Vector2I{
            static_cast<int>( std::floor( tilePosition.x / static_cast<float>( ChunkData::CHUNK_SIZE ) ) ) * ChunkData::CHUNK_SIZE,
            static_cast<int>( std::floor( tilePosition.y / static_cast<float>( ChunkData::CHUNK_SIZE ) ) ) * ChunkData::CHUNK_SIZE
        };
    }

    //* Octant position to tile position
    //* https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/
    //* Coordinates within octanct are usual cartesian
    //* Octant[0] is from vertical up CW,
    //* Octants enumerated CW
    inline Vector2I transformFromOctant(
        Vector2I const& octantPosition,
        int octant
    )
    {
        switch ( octant )
        {
            default:
            case 0:
                return { octantPosition.x, -octantPosition.y };

            case 1:
                return { octantPosition.y, -octantPosition.x };

            case 2:
                return { octantPosition.y, octantPosition.x };

            case 3:
                return { octantPosition.x, octantPosition.y };

            case 4:
                return { -octantPosition.x, octantPosition.y };

            case 5:
                return { -octantPosition.y, octantPosition.x };

            case 6:
                return { -octantPosition.y, -octantPosition.x };

            case 7:
                return { -octantPosition.x, -octantPosition.y };
        }
    }

    inline Vector2I octantToTile(
        Vector2I const& octantPosition,
        int octant,
        Vector2I const& origin
    )
    {
        return Vector2Add( origin, transformFromOctant( octantPosition, octant ) );
    }
}

#endif
