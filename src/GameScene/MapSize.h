#ifndef IG20240117233028
#define IG20240117233028

#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

class MapSize
{
private:
    RectangleExI mapsize_{};

public:
    RectangleExI const& operator()() { return mapsize_; }

    // Extends mapsize to include point provided
    void extend(Vector2I V);
    void reset();
};

#endif
