#ifndef _20240211205246
#define _20240211205246

#include "TileGenerator.h"
#include "TileMap.h"
#include <cstddef>

class World
{
public:
    TileMap& getCurrentMap();
    size_t getCurrentLevel();
    void increaseLevel();
    void decreaseLevel();

private:
    size_t maxLevel_{};
    size_t currentLevel_{};

    TileGenerator generator_{};

    std::vector<TileMap> maps_{generator_.createNewMap(maxLevel_)};
    TileMap& currentMap_{maps_.back()};

private:
    void addNewMap(size_t level);
    void setMap(size_t level);
};

#endif