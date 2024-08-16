#ifndef IG20240816170849
#define IG20240816170849

#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

/*
Idea:
- have solid Rectangles
- iterate edges
- if ray intersects solid rectangle diagonal
        point is invisible (0)
    else
        point is visible (1)
- check every tile
    sum edge visibilities
        if sum is ...
            CHOICE 1: > 1 -> tile is visible (<- Tiles.method()?) := convex edge is invisible!
            CHOICE 2: > 0 -> tile is visible (<- Tiles.method()?) := convex edge is visible!
        else
            tile is invis (<- Tiles.method()?)

*/

class Visibility
{
    struct Visibilities
    {
        std::vector<Vector2I> invisible{};
        std::vector<Vector2I> visibile{};
    };

    Visibilities visibilitiesToExport_{};

    std::vector<RectangleEx> solidRectangles_{};
    std::vector<Vector2> edgesToCheck_{};

    void importMapData(std::unordered_set<Vector2I> blocksVisions, RectangleExI const& renderRectangle);
    void setEdgesToCheck();
    void checkEdges();
    void calculateVisiblity();
    void exportVisiblity();

public:
    void update(Tiles& map, RectangleExI const& renderRectangle);
};

#endif