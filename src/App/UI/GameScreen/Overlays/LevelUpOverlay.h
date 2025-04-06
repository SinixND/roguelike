#ifndef IG20250309185723
#define IG20250309185723

#include "Panel.h"

struct Game;

class LevelUpOverlay
{
    Panel panel{};

public:
    void init();

    void update( Game const& game );
};

#endif
