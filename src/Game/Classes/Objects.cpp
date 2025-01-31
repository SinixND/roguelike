#include "Objects.h"

#include "Convert.h"
#include "DenseMap.h"
#include "EventId.h"
#include "RenderId.h"
#include "raylibEx.h"
#include <string>

void Objects::insert(
    Vector2I const& tilePosition,
    std::string const& name,
    std::string const& action,
    RenderId renderId,
    EventId event
)
{
    size_t id{ idManager.requestId() };

    ids.insert( tilePosition, id );
    positions.insert( id, Convert::tileToWorld( tilePosition ) );
    renderIds.insert( id, renderId );
    names.insert( id, name );
    actions.insert( id, action );
    events.insert( id, event );
}
