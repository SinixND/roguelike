#ifndef IG20240909162923
#define IG20240909162923

#include "TileData.h"
namespace EnemiesData
{
    struct EnemyData
    {
        int const healthBase{};
        int const damageBase{};
        float const speedBase{};
        int const scanRange{};
        int const vitality{};
        int const strength{};
        int const defense{};
        int const agility{};
    };

    EnemyData constexpr goblin{
        4,
        1,
        20 * TileData::TILE_SIZE,
        10,
        1,
        1,
        1,
        1,
    };
}

#endif
