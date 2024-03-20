#ifndef IG20240227231122
#define IG20240227231122

#include "TileTransformation.h"
#include "raylibEx.h"
#include <raylib.h>

class Position
{
public:
    Vector2& get()
    {
        return position_;
    }

    void set(Vector2 position)
    {
        position_ = position;
    }

    void set(Vector2i tilePosition)
    {
        position_ = TileTransformation::positionToWorld(tilePosition);
    }

    Vector2i tilePosition()
    {
        return TileTransformation::worldToPosition(position_);
    }

    void setTilePosition(Vector2i tilePosition)
    {
        position_ = TileTransformation::positionToWorld(tilePosition);
    }

    Position(Vector2 position = {0, 0})
        : position_(position){};

    Position(Vector2i tilePosition)
    {
        setTilePosition(tilePosition);
    }

private:
    Vector2 position_{};
};

#endif