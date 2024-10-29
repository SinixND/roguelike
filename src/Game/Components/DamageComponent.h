#ifndef IG20241029001208
#define IG20241029001208

// Unit: percent
int const BASE_CRITICAL_HIT_CHANCE{0};

// Unit: percent
int const BASE_CRITICAL_HIT_DAMAGE{50};

class DamageComponent
{
    int damage_{};

    // Unit: percent
    int criticalHitChance_{BASE_CRITICAL_HIT_CHANCE};

    // Unit: percent
    int criticalHitDamage_{BASE_CRITICAL_HIT_DAMAGE};

public:
    DamageComponent() = default;

    DamageComponent(
        int damage,
        int criticalHitChance = BASE_CRITICAL_HIT_CHANCE,
        int criticalHitDamage = BASE_CRITICAL_HIT_DAMAGE)
        : damage_(damage)
        , criticalHitChance_(criticalHitChance)
        , criticalHitDamage_(criticalHitDamage)
    {
    }

    int damage() const;
    float damageAverage() const;

    int criticalHitChancePercent() const;
    float criticalHitChanceFactor() const;

    int criticalHitDamagePercent() const;
    float criticalHitDamageFactor() const;
};

#endif