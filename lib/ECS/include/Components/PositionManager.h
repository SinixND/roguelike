#ifndef POSITIONMANAGER_H_20231212234818
#define POSITIONMANAGER_H_20231212234818

#include "Entity.h"
#include "PositionComponent.h"
#include <map>
#include <vector>

typedef size_t Index;

namespace snd
{
    class PositionManager //: public ComponentManager<Position>
    {
    public:
        Id createPosition(int x, int y);
        // Add a component to an entity
        //Position assign(Entity entity, Position position) // MOVE TO ECSManager;

        // Access a component from a specific entity
        //Position* get(Entity entity) // MOVE TO ECSManager;

        Position* getPosition(Id);

        // Remove a component from an entity
        //void unassign(Entity entity) // MOVE TO ECSManager;
        void removePosition(Id);


        // 4. Iterate over all items
        void iteratePositions(std::function<void(Position position)> lambda);

    private:
        std::vector<Position> positions_; // vector index is used as position id
        std::map<Id, Index> idToIndex_;
        //std::map<Entity, Positin*> entityToPosition; // MOVE TO ECSManager
    };
}

#endif
