#ifndef IG20240215012142
#define IG20240215012142

#include "ChunkData.h"
#include "TileData.h"
#include "raylib.h"
#include "raylibEx.h"
#include <cmath>
#include <raymath.h>

namespace UnitConversion
{
    // This file contains functions to convert between
    // - tile position (int/position, relative: considers camera)
    // - world position (float/pixel, relative: considers camera)
    // - screen position (float/pixel, absolute)
    // - chunk position (int/position), relative: considers camera)

    // World pixel to tile position
    inline Vector2I worldToTile(Vector2 const& pixel)
    {
        // World position is center of tile
        return Vector2I{
            static_cast<int>(std::floor((pixel.x + (TileData::TILE_SIZE_HALF)) / TileData::TILE_SIZE)),
            static_cast<int>(std::floor((pixel.y + (TileData::TILE_SIZE_HALF)) / TileData::TILE_SIZE))};

        // World position is top-left of tile
        // return Vector2I{
        //     static_cast<int>(std::floor(pixel.x / TileData::TILE_SIZE)),
        //     static_cast<int>(std::floor(pixel.y / TileData::TILE_SIZE))};
    }

    // Tile position to world pixel
    inline Vector2 tileToWorld(Vector2I const& tilePosition)
    {
        return Vector2{
            (tilePosition.x * TileData::TILE_SIZE),
            (tilePosition.y * TileData::TILE_SIZE)};
    }

    // Screen pixel to world pixel to tile position
    inline Vector2I screenToTile(Vector2 const& pixel, Camera2D const& camera)
    {
        Vector2 worldPixel{GetScreenToWorld2D(pixel, camera)};

        return worldToTile(worldPixel);
    }

    // Tile position to world pixel to screen pixel
    inline Vector2 tileToScreen(Vector2I const& tilePosition, Camera2D const& camera)
    {
        Vector2 worldPixel{tileToWorld(tilePosition)};

        return GetWorldToScreen2D(worldPixel, camera);
    }

    inline Vector2I getMouseTile(Camera2D const& camera)
    {
        return screenToTile(GetMousePosition(), camera);
    }

    // Tile position to chunk position
    inline Vector2I tileToChunk(Vector2I const& tilePosition)
    {
        return Vector2I{
            static_cast<int>(std::round(tilePosition.x / ChunkData::CHUNK_SIZE_I) * ChunkData::CHUNK_SIZE_I),
            static_cast<int>(std::round(tilePosition.y / ChunkData::CHUNK_SIZE_I) * ChunkData::CHUNK_SIZE_I)};
    }

    // Octant position to tile position
    // https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/
    // Coordinates within octanct are usual cartesian
    // Octant[0] is from 0,1 (top) to 1,1 (top-right), going CW
    template <typename Type>
    inline Type transformFromOctant(Type const& octantPosition, int octant)
    {
        switch (octant)
        {
        case 0:
            return Type(octantPosition.x, -octantPosition.y);
        case 1:
            return Type(octantPosition.y, -octantPosition.x);
        case 2:
            return Type(octantPosition.y, octantPosition.x);
        case 3:
            return Type(octantPosition.x, octantPosition.y);
        case 4:
            return Type(-octantPosition.x, octantPosition.y);
        case 5:
            return Type(-octantPosition.y, octantPosition.x);
        case 6:
            return Type(-octantPosition.y, -octantPosition.x);
        default:
        case 7:
            return Type(-octantPosition.x, -octantPosition.y);
        }
    }

    inline Vector2I octantToTile(Vector2I const& octantPosition, int octant, Vector2I const& origin)
    {
        return Vector2Add(origin, transformFromOctant<Vector2I>(octantPosition, octant));
    }

    inline Vector2 octantToWorld(Vector2 const& octantPosition, int octant, Vector2I const& origin)
    {
        return Vector2Add(tileToWorld(origin), transformFromOctant<Vector2>(octantPosition, octant));
    }

/*
    // Quarter position to tile position
    // Coordinates within quarter are usual cartesian
    // Quarter[0] is top-right, going CW
    template <typename Type>
    inline Type transformFromQuarter(Type const& quarterPosition, int quarter)
    {
        switch (quarter)
        {
        case 0:
            return Type(quarterPosition.x, -quarterPosition.y);
        case 1:
            return Type(quarterPosition.y, quarterPosition.x);
        case 2:
            return Type(-quarterPosition.x, quarterPosition.y);
        default:
        case 3:
            return Type(-quarterPosition.y, -quarterPosition.x);
        }
    }

    inline Vector2I quarterToTile(Vector2I const& quarterPosition, int quarter, Vector2I const& origin)
    {
        return Vector2Add(origin, transformFromQuarter<Vector2I>(quarterPosition, quarter));
    }

    inline Vector2 quarterToWorld(Vector2 const& quarterPosition, int quarter, Vector2I const& origin)
    {
        return Vector2Add(tileToWorld(origin), transformFromQuarter<Vector2>(quarterPosition, quarter));
    }
    */
}

#endif
