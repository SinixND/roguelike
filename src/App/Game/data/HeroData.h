#ifndef IG20240909134430
#define IG20240909134430

#include "TileData.h"

namespace HeroData
{
    int constexpr HEALTH_BASE{ 10 };
    int constexpr HEALTH_REGEN_BASE{ 1 };
    int constexpr DAMAGE_BASE{ 2 };
    int constexpr VISION_RANGE{ 10 };
    float constexpr SPEED_BASE{ 20 * TileData::TILE_SIZE };
}

#endif
