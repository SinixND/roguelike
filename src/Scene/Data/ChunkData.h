#ifndef IG20240815194944
#define IG20240815194944

#include "TileData.h"
#include <raylib.h>

namespace ChunkData
{
    //* Odd number of tiles to have dedicated center
    int constexpr chunkSize{25};
    float constexpr chunkSize_f{chunkSize * TileData::tileSize};
    Vector2 constexpr chunkCenter{0.5f * chunkSize_f, 0.5f * chunkSize_f};
}

#endif
