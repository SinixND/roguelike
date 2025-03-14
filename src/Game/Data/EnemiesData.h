#ifndef IG20240909162923
#define IG20240909162923

namespace EnemiesData
{
    struct EnemyData
    {
        int const healthBase{};
        int const damageBase{};
        int const scanRange{};
        int const vitality{};
        int const agility{};
    };

    EnemyData constexpr goblin{
        10,
        1,
        10,
        10,
        10,
    };
}

#endif
