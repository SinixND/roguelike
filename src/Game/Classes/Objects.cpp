#include "Objects.h"

#include "DenseMap.h"
#include "Event.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <string>
#include <utility>

void Objects::set(
    Vector2I const& tilePosition,
    RenderID renderID,
    std::string const& tag,
    Event event)
{
    positions_[tilePosition].changeTo(tilePosition);

    renderIDs_[tilePosition] = renderID;

    tags_[tilePosition] = tag;

    events_[tilePosition] = event;
}
