#ifndef IG20241029001208
#define IG20241029001208

// Unit: percent
int const baseCriticalHitChance{0};

// Unit: percent
int const baseCriticalHitDamage{50};

class DamageComponent
{
    int baseDamage_{};

    // Unit: percent
    int criticalHitChance_{baseCriticalHitChance};

    // Unit: percent
    int criticalHitDamage_{baseCriticalHitDamage};

public:
    DamageComponent() = default;

    DamageComponent(
        int damage,
        int criticalHitChance = baseCriticalHitChance,
        int criticalHitDamage = baseCriticalHitDamage)
        : baseDamage_(damage)
        , criticalHitChance_(criticalHitChance)
        , criticalHitDamage_(criticalHitDamage)
    {
    }

    int baseDamage() const;

    int damage() const;
    float damageAverage() const;

    int criticalHitChancePercent() const;
    float criticalHitChanceFactor() const;

    int criticalHitDamagePercent() const;
    float criticalHitDamageFactor() const;
};

#endif