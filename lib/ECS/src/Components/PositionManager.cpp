#include "PositionManager.h"
#include "Id.h"
#include "IdManager.h"
#include <iostream>
#include <vector>
#include <map>

namespace snd
{
    // template <typename... ArgTypes>
    // function(ArgTypes... args){
        // subfunction(args...);
    // }
    Id PositionManager::createPosition(int x, int y){
        IdManager* idManager{IdManager::getInstance()};
        Id positionId = idManager.requestId();

        Index positionIndex = positions_.size();
        positions_.push_back(Position(x, y));
        lastId_ = positionId;
        idToIndex_.create(std::make_pair(positionId, positionIndex));

        return positionId;
    };

    Position* PositionManager::get(Id id)
    {
        return &positions_[idToIndex_[id]];
    };

    void PositionManager::removePosition(Id removedPositionId){
        if (!positions_.size()) { return };

        Index lastPositionIndex = positions_.size() - 1;
        Id keptPositionId = positions_[lastPositionIndex].id_;

        Index removedPositionIndex = idToIndex_[removedPositionId];

        // swap before popping if more than one element to keep elements contiguous  
        if (positions_.size() > 1)
        {
            // swap positions (by index) so last entry can be popped
            positions_[removedPositionIndex] = positions_[lastPositionIndex];
            // update mapping (by id)
            idToIndex_[keptPositionId] = removedPositionIndex;
            // remove removed id from mapping
            idToIndex_.erase(removedPositionId);
        }

        positions_.pop_back();
    };

    void PositionManager::iteratePositions(std::function<void(Position position)> lambda){
        for (auto& position : positions_)
        {
            lambda(position);
        };
    };
}
