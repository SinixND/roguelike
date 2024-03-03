#ifndef _20240227231122
#define _20240227231122

#include "TilePosition.h"
#include "TilePositionConversion.h"
#include "raylibEx.h"

class Position
{
public:
    Vector2& get()
    {
        return position_;
    }

    void set(const Vector2& position)
    {
        position_ = position;
    }

    void set(const TilePosition& tilePosition)
    {
        position_ = tilePositionToWorld(tilePosition);
    }

    TilePosition tilePosition()
    {
        return worldToTilePosition(position_);
    }

    void setTilePosition(const TilePosition& tilePosition)
    {
        position_ = tilePositionToWorld(tilePosition);
    }

    Position(const Vector2& position = {0, 0})
        : position_(position){};

    Position(const TilePosition& tilePosition)
    {
        setTilePosition(tilePosition);
    }

private:
    Vector2 position_{};
};

#endif