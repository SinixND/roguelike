#ifndef IG20240308005429
#define IG20240308005429

#include <raylib.h>

namespace Zoom
{
    void update(float wheelMove, Camera2D& camera);
    void reset(Camera2D& camera);
}

#endif