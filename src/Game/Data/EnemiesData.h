#ifndef IG20240909162923
#define IG20240909162923

namespace EnemiesData
{
    struct EnemyData
    {
        int const healthBase{};
        int const damageBase{};
        int const scanRange{};
    };

    EnemyData goblin{
        5,
        1,
        10
    };
}

#endif
