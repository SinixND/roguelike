#ifndef IG20240909162923
#define IG20240909162923

namespace EnemiesData
{
    struct EnemyData
    {
        // int const energyMax{};
        int const energyRegenBase{};
        int const agilityBase{};
        int const scanRange{};
        int const healthBase{};
        int const damageBase{};
    };

    EnemyData goblin{
        // 2,
        1,
        1,
        10,
        5,
        1
    };
}

#endif
