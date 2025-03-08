#ifndef IG20240909162923
#define IG20240909162923

namespace EnemiesData
{
    struct EnemyData
    {
        int energyMax{};
        int energyRegenBase{};
        int agilityBase{};
        int scanRange{};
        int healthBase{};
        int damageBase{};
    };

    EnemyData goblin{
        2,
        1,
        1,
        10,
        5,
        1
    };
}

#endif
