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

snx::DenseMap<Vector2I, Position> const& Objects::positions() const
{
    return positions_;
}

Position const& Objects::position(Vector2I const& tilePosition) const
{
    return positions_.at(tilePosition);
}

snx::DenseMap<Vector2I, std::string> const& Objects::tags() const
{
    return tags_;
}

std::string const& Objects::tag(Vector2I const& tilePosition) const
{
    return tags_.at(tilePosition);
}

snx::DenseMap<Vector2I, RenderID> const& Objects::renderIDs() const
{
    return renderIDs_;
}

RenderID Objects::renderID(Vector2I const& tilePosition) const
{
    return renderIDs_.at(tilePosition);
}

snx::DenseMap<Vector2I, Event> const& Objects::events() const
{
    return events_;
}

Event Objects::event(Vector2I const& tilePosition) const
{
    return events_.at(tilePosition);
}

size_t Objects::size() const
{
    return positions_.size();
}
