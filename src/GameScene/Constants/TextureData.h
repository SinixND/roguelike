#ifndef IG20240414161450
#define IG20240414161450

#include <raylib.h>
#include <string>

namespace TextureData
{
    std::string const TEXTURE_ATLAS_FILE{"TextureAtlas.png"};
    float constexpr TEXTURE_SIZE{32};
    // Vector2 constexpr TEXTURE_DIMENSIONS{TEXTURE_SIZE, TEXTURE_SIZE};

    float constexpr TILE_SIZE{25};
    Vector2 constexpr TILE_CENTER{TILE_SIZE / 2, TILE_SIZE / 2};
    // Vector2 constexpr TILE_DIMENSIONS{TILE_SIZE, TILE_SIZE};
}

#endif
