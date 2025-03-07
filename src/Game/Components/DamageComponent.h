#ifndef IG20241029001208
#define IG20241029001208

struct DamageComponent
{
    int baseDamage{};
    float critChanceFactor{ 0.0f };
    float critDamageFactor{ 2.0f };
};

namespace DamageModule
{
    int damageRNG( DamageComponent const& damageComponent );
    int damageAverage( DamageComponent const& damageComponent );
}

#endif
