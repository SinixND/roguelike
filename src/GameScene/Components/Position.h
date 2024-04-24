#ifndef IG20240227231122
#define IG20240227231122

#include "TileTransformation.h"
#include "raylibEx.h"
#include <raylib.h>

class PositionComponent
{
public:
    Vector2 renderPosition() const { return position_; }

    void setRenderPosition(Vector2 position) { position_ = position; }

    Vector2I tilePosition() const
    {
        return TileTransformation::worldToPosition(position_);
    }

    void setTilePosition(Vector2I tilePosition)
    {
        position_ = TileTransformation::positionToWorld(tilePosition);
    }

    bool hasPositionChanged()
    {
        if (!Vector2Equals(oldTilePosition_, tilePosition()))
        {
            hasPositionChanged_ = true;
        }

        return hasPositionChanged_;
    }

    void resetPositionChanged()
    {
        oldTilePosition_ = tilePosition();
        hasPositionChanged_ = false;
    }

    explicit PositionComponent(Vector2 position = {0, 0})
        : position_(position)
    {
    }

    explicit PositionComponent(Vector2I tilePosition)
    {
        setTilePosition(tilePosition);
    }

private:
    Vector2 position_{};
    Vector2I oldTilePosition_{tilePosition()};
    bool hasPositionChanged_{true};
};

#endif