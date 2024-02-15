#ifndef _20240211205246
#define _20240211205246

#include "MapHandleService.h"
#include "TileMap.h"
#include <cstddef>

class World
{
public:
    TileMap& currentMap();
    size_t currentLevel();
    void increaseLevel();
    void decreaseLevel();

private:
    size_t maxLevel_{};
    size_t currentLevel_{};

    MapHandleService mapHandler_{};

    std::vector<TileMap> maps_{mapHandler_.createNewMap(maxLevel_)};
    TileMap& currentMap_{maps_.back()};

private:
    void addNewMap(size_t level);
    void setMap(size_t level);
};

#endif