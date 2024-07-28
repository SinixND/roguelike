#ifndef IG20240727170919
#define IG20240727170919

#include "RenderID.h"
#include "Tiles.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

class TilesToRender
{
    std::vector<Vector2> renderPositions_{};
    std::vector<RenderID> renderIDs_{};
    std::vector<VisibilityID> visibilityIDs_{};

public:
    void insert(Tiles& currentMap, Vector2I const& tilePosition);
    void clear();

    std::vector<Vector2> renderPositions() { return renderPositions_; }
    std::vector<RenderID> renderIDs() { return renderIDs_; }
    std::vector<VisibilityID> visibilityIDs() { return visibilityIDs_; }
};

#endif