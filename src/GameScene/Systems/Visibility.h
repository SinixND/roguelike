#ifndef IG20240816170849
#define IG20240816170849

#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>
#include <unordered_map>
#include <vector>

/*
Idea:
- have solid Rectangles
- iterate corners
- if ray intersects solid rectangle diagonal
        point is invisible (0)
    else
        point is visible (1)
- check every tile
    sum corner visibilities
        if sum is ...
            CHOICE 1: > 1 -> tile is visible (<- Tiles.method()?) := convex corner is invisible!
            CHOICE 2: > 0 -> tile is visible (<- Tiles.method()?) := convex corner is visible!
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

    Visibilities visibilities_{};

    std::vector<Vector2> cornersToCheck_{};

    // false = no visible corner, true = at least one visible corner
    // unordered_set is sufficient, if tile gets visible at first visible corner
    std::unordered_map<Vector2I, bool> tilesToCheck_{};

    std::vector<RectangleEx> solidRectangles_{};

private:
    void importMapData(
        Tiles& map,
        RectangleEx const& mapPanel,
        Camera2D const& camera);

    void importSolidRectangles(
        Tiles& map,
        RectangleEx const& mapPanel,
        Camera2D const& camera);

    void setTargetsToCheck(
        RectangleEx const& mapPanel,
        Camera2D const& camera);

    // erases invisible corners
    void checkCorners(Vector2I const& heroPosition, Camera2D const& camera);
    void calculateVisiblity();
    void exportVisiblity(Tiles& map);

    void reset();

public:
    void update(
        Tiles& map,
        RectangleEx const& mapPanel,
        Camera2D const& camera,
        Vector2I const& heroPosition);

    Visibilities const& visibilities() { return visibilities_; }
};

#endif