#ifndef _20240211205246
#define _20240211205246

#include "MapHandler.h"
#include "MapOverlayHandler.h"
#include "TileMap.h"
#include <cstddef>

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

    MapHandler mapHandler_{};
    MapOverlayHandler mapOverlayHandler_{};

    std::vector<TileMap> maps_{mapHandler_.createNewMap(maxLevel_)};
    TileMap& currentMap_{maps_.back()};

    TileMap mapOverlay_{};
    TileMap framedMapOverlay_{};

private:
    void addNewMap(size_t level);
    void setMap(size_t level);
};

#endif