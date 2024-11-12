#include "Objects.h"
#include "DenseMap.h"
#include "EventID.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <string>

void Objects::set(
    Vector2I const& tilePosition,
    RenderID renderID,
    std::string const& name,
    std::string const& action,
    EventID event)
{
    positions[tilePosition].changeTo(tilePosition);

    renderIDs[tilePosition] = renderID;

    names[tilePosition] = name;

    actions[tilePosition] = action;

    events[tilePosition] = event;
}
