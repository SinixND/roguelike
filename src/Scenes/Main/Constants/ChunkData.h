#ifndef IG20240815194944
#define IG20240815194944

#include "TileData.h"
#include <raylib.h>

namespace ChunkData
{
    //* Odd number of tiles to have dedicated center
    int constexpr CHUNK_SIZE_I{25};
    float constexpr CHUNK_SIZE_F{CHUNK_SIZE_I * TileData::TILE_SIZE};
    Vector2 constexpr CHUNK_CENTER{CHUNK_SIZE_F / 2, CHUNK_SIZE_F / 2};
}

#endif
