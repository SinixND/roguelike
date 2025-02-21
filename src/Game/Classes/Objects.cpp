#include "Objects.h"

#include "Convert.h"
#include "DenseMap.h"
#include "Events.h"
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
    size_t id{ idManager_.requestId() };

    ids.insert( tilePosition, id );
    positions.insert( id, Convert::tileToWorld( tilePosition ) );
    renderIds.insert( id, renderId );
    names.insert( id, name );
    actions.insert( id, action );
    events.insert( id, event );
}
