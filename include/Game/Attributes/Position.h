#ifndef POSITION_H_20240128153104
#define POSITION_H_20240128153104

#include "Utility.h"
#include "raylibEx.h"

class Position
{
public:
    int x;
    int y;

public:
    void updateFromMousePosition()
    {
        Vector2Int position{
            getScreenToTile(GetMousePosition())};

        x = position.x;
        y = position.y;
    }
};

#endif