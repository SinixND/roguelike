#ifndef _20240211172120
#define _20240211172120

#include "Position.h"
#include "RenderId.h"

class GameObject
{
public:
    Position& position() { return position_; };
    void setPosition(Position position) { position_ = position; };

    RenderId& renderId() { return renderId_; };
    void setRenderId(RenderId renderId) { renderId_ = renderId; };

private:
    Position position_{};
    RenderId renderId_{};
};

#endif