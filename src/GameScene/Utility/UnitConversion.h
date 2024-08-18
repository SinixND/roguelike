#ifndef IG20240215012142
#define IG20240215012142

#include "ChunkData.h"
#include "Debugger.h"
#include "TileData.h"
#include "raylib.h"
#include "raylibEx.h"
#include <cmath>
#include <string>

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
        return Vector2I{
            static_cast<int>(std::floor((pixel.x + (TileData::TILE_SIZE_HALF)) / TileData::TILE_SIZE)),
            static_cast<int>(std::floor((pixel.y + (TileData::TILE_SIZE_HALF)) / TileData::TILE_SIZE))};
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
        Vector2 worldPixel{
            tilePosition.x * TileData::TILE_SIZE,
            tilePosition.y * TileData::TILE_SIZE};

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
    // Octant[0] is from 0,1 (top) to 1,1 (top-right)
    inline Vector2I transformFromOctant(Vector2I const& octantPosition, int octant)
    {
        switch (octant)
        {
        case 0:
            return Vector2I(octantPosition.x, -octantPosition.y);
        case 1:
            return Vector2I(octantPosition.y, -octantPosition.x);
        case 2:
            return Vector2I(octantPosition.y, octantPosition.x);
        case 3:
            return Vector2I(octantPosition.x, octantPosition.y);
        case 4:
            return Vector2I(-octantPosition.x, octantPosition.y);
        case 5:
            return Vector2I(-octantPosition.y, octantPosition.x);
        case 6:
            return Vector2I(-octantPosition.y, -octantPosition.x);
        default:
        case 7:
            return Vector2I(-octantPosition.x, -octantPosition.y);
        }
    }

    inline Vector2I octantToTile(Vector2I const& octantPosition, int octant, Vector2I const& origin)
    {
#ifdef DEBUG
        // auto v = Vector2Add(origin, transformFromOctant(octantPosition, octant));
        // snx::dbg::cliLog("Octant[" + std::to_string(octant) + "]: " + std::to_string(octantPosition.x) + ", " + std::to_string(octantPosition.y) + " to Tile: " + std::to_string(v.x) + ", " + std::to_string(v.y) + "\n");
#endif
        return Vector2Add(origin, transformFromOctant(octantPosition, octant));
    }
}

#endif
