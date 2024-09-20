#ifndef IG20240531143458
#define IG20240531143458

#include "Energy.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"

class Hero
{
    RenderID renderID_{RenderID::hero};

public:
    Movement movement_{};
    Energy energy_{};
    Position position_{};

public:
    void init();

    // Getter / Setter
    RenderID renderID() const;
};

#endif
