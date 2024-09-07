#ifndef IG20240907173344
#define IG20240907173344

#include "Tiles.h"
#include "TilesActive.h"
#include <vector>

class Map
{
    std::vector<Tiles> passive_{};
    std::vector<TilesActive> active_{};

public:
    std::vector<Tiles>& passive() { return passive_; };
    std::vector<TilesActive>& active() { return active_; };
};

#endif