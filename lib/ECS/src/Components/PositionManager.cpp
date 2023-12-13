#include "PositionManager.h"
#include "Id.h"

namespace snd
{
    Position createPosition(int x, int y){
        Id id = std::static_cast<Id>( positions_.size() );
        positions_.push_back(Position(x,y));
        return id;
    };

    void removePosition(Id id){
        //positions_.swap();
        //position_.pop_back();
    };

        void iteratePositions(std::function<void(Position position)> lambda){};

}
