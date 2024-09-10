#include "Objects.h"

#include "DenseMap.h"
#include "Event.h"
#include "Position.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <cstddef>
#include <string>

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

snx::DenseMap<Vector2I, Position>& Objects::positions()
{
    return positions_;
}

Position const& Objects::position(Vector2I const& tilePosition)
{
    return positions_[tilePosition];
}

snx::DenseMap<Vector2I, std::string>& Objects::tags()
{
    return tags_;
}
std::string const& Objects::tag(Vector2I const& tilePosition)
{
    return tags_[tilePosition];
}

snx::DenseMap<Vector2I, RenderID>& Objects::renderIDs()
{
    return renderIDs_;
}

RenderID Objects::renderID(Vector2I const& tilePosition)
{
    return renderIDs_[tilePosition];
}

snx::DenseMap<Vector2I, Event>& Objects::events()
{
    return events_;
}

Event Objects::event(Vector2I const& tilePosition)
{
    return events_[tilePosition];
}

size_t Objects::size()
{
    return positions_.size();
}
