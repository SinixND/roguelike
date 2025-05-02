#include "WaitSystem.h"

#include "ActionId.h"
#include "Enemies.h"
#include "EnergyComponent.h"
#include "Hero.h"

namespace WaitSystem
{
    void update(
        Hero& heroIO,
        Enemies& enemiesIO
    )
    {
        if ( *heroIO.action == ActionId::WAIT )
        {
            heroIO.energy = EnergyModule::exhaust( heroIO.energy );
        }

        for ( size_t idx{ 0 }; idx < enemiesIO.actions.size(); ++idx )
        {
            size_t enemyId{ enemiesIO.actions.key( idx ) };

            if ( enemiesIO.actions[idx] == ActionId::WAIT )
            {
                enemiesIO.energies.at( enemyId ) = EnergyModule::exhaust( enemiesIO.energies.at( enemyId ) );
            }
        }
    }
}
