#include "InteractSystem.h"

#include "Hero.h"
#include "Objects.h"

namespace InteractSystem
{
    void update(
        Hero& heroIO,
        Objects const& objectsIO
    )
    {
        if ( *heroIO.action == ActionId::INTERACT )
        {
            heroIO.energy = EnergyModule::exhaust( heroIO.energy );

            heroIO.action.reset();
        }
    }
}
