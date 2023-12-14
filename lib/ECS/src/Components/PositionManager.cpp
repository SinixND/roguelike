#include "PositionManager.h"
#include "Id.h"
#include <iostream>

namespace snd
{
    // template <typename... ArgTypes>
    // function(ArgTypes... args){
        // subfunction(args...);
    // }
    Id PositionManager::createPosition(int x, int y){
        Id id = static_cast<Id>( positions_.size() );
        positions_.push_back(Position(x, y));
        std::cout << "Id "<< id << " points to (" << positions_[id].x_ << ", " << positions_[id].y_ << "); should be (" << x << ", " << y << ")\n";
        return id;
    };

    Id PositionManager::removePosition(Id id){
        Id swappedId = positions_.size() - 1;
        positions_[id] = positions_[swappedId];
        positions_.pop_back();

        return swappedId; // return (popped) id to be updated (to removed id)

    };

    void PositionManager::iteratePositions(std::function<void(Position position)> lambda){
        for (auto& position : positions_)
        {
            lambda(position);
        };
    };
}
