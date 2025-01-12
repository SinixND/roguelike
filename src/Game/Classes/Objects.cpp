#include "Objects.h"
#include "Convert.h"
#include "DenseMap.h"
#include "EventId.h"
#include "RenderId.h"
#include "raylibEx.h"
#include <string>

namespace ObjectsModule
{
    void createSingle(
        Objects& objects,
        Vector2I const& tilePosition,
        RenderId renderId,
        std::string const& name,
        std::string const& action,
        EventId event)
    {
        objects.positions[tilePosition] = Convert::tileToWorld(tilePosition);

        objects.renderIds[tilePosition] = renderId;

        objects.names[tilePosition] = name;

        objects.actions[tilePosition] = action;

        objects.events[tilePosition] = event;
    }
}
