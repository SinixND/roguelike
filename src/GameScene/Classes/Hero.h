#ifndef IG20240531143458
#define IG20240531143458

#include "Position.h"
#include "RenderID.h"

class Hero
{
    Position position_{};
    RenderID renderID_{RenderID::HERO};

public:
    Position& position() { return position_; };

    RenderID const& renderID() const { return renderID_; };
};

#endif