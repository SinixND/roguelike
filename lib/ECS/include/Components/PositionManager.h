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
        //Position assign(Entity entity, Position position) // MOVE TO ECSManager;
        Position createPosition();

        // Access a component from a specific entity
        //Position* get(Entity entity) // MOVE TO ECSManager;

        // Remove a component from an entity
        //void unassign(Entity entity) // MOVE TO ECSManager;
        void removePosition(Position);


        // 4. Iterate over all items
        void iteratePositions(std::function<void(Position position)> lambda);

    private:
        std::vector<Position> positions{}; // vector index is used as position id
        //std::map<Entity, Positin*> entityToPosition; // MOVE TO ECSManager
    };
}

#endif
