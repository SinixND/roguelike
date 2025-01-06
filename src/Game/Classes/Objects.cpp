#include "Objects.h"
#include "Convert.h"
#include "EventId.h"
#include "RenderId.h"
#include "SparseSet.h"
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
        size_t objectId{0};

        if (!objects.ids.contains(tilePosition))
        {
            objectId = objects.idManager.requestId();
            objects.ids.insert(tilePosition, objectId);
        }
        else
        {
            objectId = objects.ids.at(tilePosition);
        }

        objects.ids.insert(tilePosition, objectId);

        objects.positions[objectId] = Convert::tileToWorld(tilePosition);

        objects.renderIds[objectId] = renderId;

        objects.names[objectId] = name;

        objects.actions[objectId] = action;

        objects.events[objectId] = event;
    }
}
