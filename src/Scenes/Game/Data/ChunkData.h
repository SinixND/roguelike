#ifndef IG20240815194944
#define IG20240815194944

#include "TileData.h"
#include <raylib.h>

namespace ChunkData
{
    //* Odd number of tiles to have dedicated center
    int constexpr CHUNK_SIZE{ 25 };
    float constexpr CHUNK_SIZE_F{ CHUNK_SIZE * TileData::TILE_SIZE };
    Vector2 constexpr CHUNK_CENTER{ 0.5f * CHUNK_SIZE_F, 0.5f * CHUNK_SIZE_F };
}

#endif
