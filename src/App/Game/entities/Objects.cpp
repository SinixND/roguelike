#include "Objects.h"

#include "Convert.h"
#include "DenseMap.h"
#include "EventId.h"
#include "RenderId.h"
#include "raylibEx.h"
#include <string>

namespace ObjectsModule
{
    Objects const& insert(
        Objects& objects,
        Vector2I const& tilePosition,
        std::string const& name,
        std::string const& action,
        RenderId renderId,
        EventId event
    )
    {
        size_t id{ Objects::idManager_.requestId() };

        objects.ids.insert( tilePosition, id );
        objects.positions.insert( id, Convert::tileToWorld( tilePosition ) );
        objects.renderIds.insert( id, renderId );
        objects.names.insert( id, name );
        objects.actions.insert( id, action );
        objects.eventIds.insert( id, event );

        return objects;
    }
}
