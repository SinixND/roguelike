#include "InteractSystem.h"

#include "Convert.h"
#include "EventDispatcher.h"
#include "Hero.h"
#include "Objects.h"

namespace InteractSystem
{
    void update(
        Hero& heroIO,
        Objects const& objects
    )
    {
        if (
            heroIO.action
            && *heroIO.action == ActionId::INTERACT
        )
        {
            size_t objectId{ objects.ids.at( Convert::worldToTile( heroIO.position ) ) };

            //* Interact if possible
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
            snx::Debugger::cliLog( "Hero interacts.\n" );
#endif
            snx::EventDispatcher::notify( objects.eventIds.at( objectId ) );

            heroIO.action.reset();
        }
    }
}
