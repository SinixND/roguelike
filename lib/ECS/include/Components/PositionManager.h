#ifndef POSITIONMANAGER_H_20231212234818
#define POSITIONMANAGER_H_20231212234818

#include "Entity.h"
#include "PositionComponent.h"
#include <functional>
#include <map>
#include <vector>

typedef size_t Index;

namespace snd
{
    class PositionManager //: public ComponentManager<Position>
    {
    public:
        // Add a component to an entity
        void assignPosition(Entity entity, Position position);

        // Access a component from a specific entity
        Position* getPosition(Entity entity);

        // Remove a component from an entity
        void removePosition(Entity entity);

        // 4. Iterate over all items
        void iteratePositions(std::function<void(Position position)> lambda);

    private:
        std::vector<Position> positions_;          // vector index is used as position id
        std::map<Entity, Index> entityToPosition_; // entity is used to identify position, because position cant exist alone
        std::map<Index, Entity> positionToEntity_; // store a position to entity mapping for removing
    };
}

#endif
