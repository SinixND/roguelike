#include "DenseMap.h"
#include "Event.h"
#include "ObjectSoA.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <string>

void ObjectSoA::set(
    Vector2I const& tilePosition,
    RenderID renderID,
    std::string const& name,
    std::string const& action,
    Event event)
{
    positions[tilePosition].changeTo(tilePosition);

    renderIDs[tilePosition] = renderID;

    names[tilePosition] = name;

    actions[tilePosition] = action;

    events[tilePosition] = event;
}
