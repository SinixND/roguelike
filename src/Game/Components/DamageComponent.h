#ifndef IG20241029001208
#define IG20241029001208

#include "DamageData.h"

struct DamageComponent
{
    int baseDamage{};
    float critChanceFactor{ DamageData::baseCritChance };
    float critDamageFactor{ DamageData::baseCritDamage };
};

namespace DamageModule
{
    int damageRNG( DamageComponent const& damageComponent );
    int damageAverage( DamageComponent const& damageComponent );
}

#endif
