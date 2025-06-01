#include "WaitSystem.h"

#include "ActionId.h"
#include "Enemies.h"
#include "Hero.h"

namespace WaitSystem
{
    void update(
        Hero& heroIO,
        Enemies& enemiesIO
    )
    {
        if (
            heroIO.action
            && *heroIO.action == ActionId::WAIT
        )
        {
            heroIO.health = HealthModule::regenerate( heroIO.health );

            heroIO.action.reset();
        }

        for ( size_t idx{ 0 }; idx < enemiesIO.actions.size(); ++idx )
        {
            size_t enemyId{ enemiesIO.actions.key( idx ) };

            if ( enemiesIO.actions[idx] == ActionId::WAIT )
            {
                enemiesIO.actions.erase( enemyId );
            }
        }
    }
}
