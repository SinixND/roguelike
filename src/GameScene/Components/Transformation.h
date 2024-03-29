#ifndef IG20240227231122
#define IG20240227231122
#include "TileTransformation.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

class Transformation
{
public:
    Vector2 position() const { return position_; }

    void setPosition(Vector2 position) { position_ = position; }

    Vector2i tilePosition() const
    {
        return TileTransformation::worldToPosition(position_);
    }

    void setTilePosition(Vector2i tilePosition)
    {
        position_ = TileTransformation::positionToWorld(tilePosition);
    }

    bool hasPositionChanged()
    {
        if (!Vector2Equals(oldPosition_, tilePosition())) {
            hasPositionChanged_ = true;
        }

        return hasPositionChanged_;
    }

    void resetPositionChanged()
    {
        oldPosition_ = tilePosition();
        hasPositionChanged_ = false;
    }

    Transformation(Vector2 position = {0, 0})
        : position_(position)
    {}

    Transformation(Vector2i tilePosition) { setTilePosition(tilePosition); }

private:
    Vector2 position_{};
    Vector2i oldPosition_{tilePosition()};
    bool hasPositionChanged_{true};
};

#endif