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

snx::DenseMap<Vector2I, PositionComponent> const& Objects::getPositions() const
{
    return positions_;
}

snx::DenseMap<Vector2I, RenderID> const& Objects::getRenderIDs() const
{
    return renderIDs_;
}

snx::DenseMap<Vector2I, RenderID>& Objects::getRenderIDs()
{
    return const_cast<snx::DenseMap<Vector2I, RenderID>&>(std::as_const(*this).getRenderIDs());
}

snx::DenseMap<Vector2I, std::string> const& Objects::getTags() const
{
    return tags_;
}

snx::DenseMap<Vector2I, std::string>& Objects::getTags()
{
    return const_cast<snx::DenseMap<Vector2I, std::string>&>(std::as_const(*this).getTags());
}

snx::DenseMap<Vector2I, Event> const& Objects::getEvents() const
{
    return events_;
}

snx::DenseMap<Vector2I, Event>& Objects::getEvents()
{
    return const_cast<snx::DenseMap<Vector2I, Event>&>(std::as_const(*this).getEvents());
}
