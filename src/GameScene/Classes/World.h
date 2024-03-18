#ifndef IG20240211205246
#define IG20240211205246

#include "MapHandler.h"
#include "TileMap.h"
#include <cstddef>
#include <vector>

class World
{
public:
    TileMap& currentMap();
    size_t currentLevel();

    void increaseLevel();
    void decreaseLevel();

    TileMap& mapOverlay();
    TileMap& framedMapOverlay();

private:
    size_t maxLevel_{};
    size_t currentLevel_{};

    std::vector<TileMap> maps_{MapHandler::createNewMap(maxLevel_)};
    TileMap* currentMap_{&maps_.back()};

    TileMap mapOverlay_{};
    TileMap framedMapOverlay_{};

private:
    void addNewMap(size_t level);
    void setMap(size_t level);
};

#endif