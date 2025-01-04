#include "Objects.h"
#include "DenseMap.h"
#include "EventId.h"
#include "PositionComponent.h"
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
        PositionModule::changeTo(
            objects.positions[tilePosition],
            tilePosition);

        objects.renderIds[tilePosition] = renderId;

        objects.names[tilePosition] = name;

        objects.actions[tilePosition] = action;

        objects.events[tilePosition] = event;
    }
}
