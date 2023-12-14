#include "PositionManager.h"

#include "Entity.h"
#include "PositionComponent.h"
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace snd
{
    // template <typename... ArgTypes>
    // function(ArgTypes... args){
    // subfunction(args...);
    // }
    void PositionManager::assignPosition(Entity entity, Position position)
    {
        Index positionIndex = positions_.size();

        positions_.push_back(position);
        entityToPosition_.insert(std::make_pair(entity, positionIndex));
        positionToEntity_.insert(std::make_pair(positionIndex, entity));
    };

    Position* PositionManager::getPosition(Entity entity)
    {
        return &positions_[entityToPosition_[entity]];
    };

    void PositionManager::removePosition(Entity entity)
    {
        std::cout << "map end: " << entityToPosition_.find(entity)->first << "\n";

        if (!entityToPosition_.find(entity)->first)
        {
            return;
        };

        Index removedPositionIndex = entityToPosition_[entity];

        // replace with last component before popping if more than one element to keep elements contiguous
        if (positions_.size() > 1)
        {
            Index lastPositionIndex = positions_.size() - 1;
            Entity keptEntity = positionToEntity_[lastPositionIndex];

            // swap positions (by index) so last entry can be popped
            positions_[removedPositionIndex] = positions_[lastPositionIndex];
            // update mapping (by id)
            entityToPosition_[keptEntity] = removedPositionIndex;
        }

        // remove removed entity from mapping
        entityToPosition_.erase(entity);
        // remove removed position from mapping
        positionToEntity_.erase(removedPositionIndex);

        positions_.pop_back();
    };

    void PositionManager::iteratePositions(std::function<void(Position position)> lambda)
    {
        for (auto& position : positions_)
        {
            lambda(position);
        };
    };
}
