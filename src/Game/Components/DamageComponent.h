#ifndef IG20241029001208
#define IG20241029001208

// Unit: percent
int const baseCriticalHitChance{0};

// Unit: percent
int const baseCriticalHitDamage{50};

struct DamageComponent
{
    int baseDamage{0};

    // Unit: percent
    int criticalHitChance{baseCriticalHitChance};

    // Unit: percent
    int criticalHitDamage{baseCriticalHitDamage};
};

int calculateDamage(DamageComponent const& damageComponent);

float damageAverage(DamageComponent const& damageComponent);

#endif
