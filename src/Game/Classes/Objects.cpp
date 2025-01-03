#include "Objects.h"

#include "EventId.h"
#include "RenderId.h"
#include "raylibEx.h"
#include <string>

void createObject(
    Objects& objects,
    Vector2I const& tilePosition,
    RenderId renderId,
    std::string const& name,
    std::string const& action,
    EventId event)
{
    size_t id{objects.idManager.requestId()};

    objects.positions.emplace(id, tilePosition);
    objects.renderIds.insert(id, renderId);
    objects.names.insert(id, name);
    objects.actions.insert(id, action);
    objects.events.insert(id, event);
}
