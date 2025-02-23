#include "DamageComponent.h"

#define DEBUG_DAMAGE

#include "RNG.h"
#include <raylib.h>

#if defined( DEBUG ) && defined( DEBUG_DAMAGE )
#include "Debugger.h"
#endif

namespace DamageModule
{
    int damageRNG( DamageComponent const& damageComponent )
    {
        int damage = damageComponent.baseDamage
                     + ( damageComponent.baseDamage
                         * ( snx::RNG::random( 0.0f, 100.0f )
                             < damageComponent.critChanceFactor )
                         * damageComponent.critDamageFactor );

#if defined( DEBUG )
        snx::debug::cliPrint( TextFormat( "%i damage", damage ) );
#endif

        return damage;
    }

    int damageAverage( DamageComponent const& damageComponent )
    {
        return damageComponent.baseDamage
               + ( damageComponent.baseDamage
                   * damageComponent.critChanceFactor
                   * damageComponent.critDamageFactor );
    }
}
