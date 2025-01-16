#ifndef IG20241029001208
#define IG20241029001208

float const baseCritChance{ 0.0f };

float const baseCritDamage{ 0.50f };

struct DamageComponent
{
    int baseDamage{};
    float critChanceFactor{ baseCritChance };
    float critDamageFactor{ baseCritDamage };
};

namespace DamageModule
{
    int damageRNG( DamageComponent const& damageComponent );
    int damageAverage( DamageComponent const& damageComponent );
}

#endif
