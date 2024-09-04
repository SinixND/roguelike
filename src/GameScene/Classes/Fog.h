#ifndef IG20240904175147
#define IG20240904175147

#include "raylibEx.h"

class Fog
{
    Vector2I tilePosition_{};
    bool isFogOpaque_{};

public:
    Vector2I const& tilePosition() const { return tilePosition_; }
    void setTilePosition(Vector2I const& tilePosition) { tilePosition_ = tilePosition; }

    bool isFogOpaque() const { return isFogOpaque_; }
    void setFogOpaque(bool isFogOpaque) { isFogOpaque_ = isFogOpaque; }

    Fog() = default;

    Fog(Vector2I const& tilePosition, bool isFogOpaque)
        : tilePosition_(tilePosition)
        , isFogOpaque_(isFogOpaque)
    {
    }
};

#endif
