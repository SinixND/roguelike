#include "MapSize.h"

void MapSize::extend(Vector2I V)
{
    auto left{
        (V.x < mapsize_.left)
            ? V.x
            : mapsize_.left};

    auto right{
        (V.x > mapsize_.right)
            ? V.x
            : mapsize_.right};

    auto top{
        (V.y < mapsize_.top)
            ? V.y
            : mapsize_.top};

    auto bottom{
        (V.y > mapsize_.bottom)
            ? V.y
            : mapsize_.bottom};

    mapsize_ = RectangleExI{
        Vector2I{
            left,
            top},
        Vector2I{
            right,
            bottom}};
}

void MapSize::reset()
{
    mapsize_ = {0, 0};
}