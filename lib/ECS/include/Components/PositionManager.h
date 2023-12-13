#ifndef POSITIONMANAGER_H_20231212234818
#define POSITIONMANAGER_H_20231212234818

#include "Entity.h"
#include "PositionComponent.h"
#include <map>
#include <vector>

namespace snd
{
    class PositionManager //: public ComponentManager<Position>
    {
    public:
        // Add a component to an entity
        Position assign(Entity entity, Position position);

        // Access a component from a specific entity
        Position* get(Entity entity);

        // Remove a component from an entity
        void remove(Entity entity);

        // 4. Iterate over all items

    private:
        std::vector<Position> components; // positions aka position components
        std::map<Entity, Position*> entityMap;
    };
}

#endif