#ifndef IG20240308002046
#define IG20240308002046

#include "TextureData.h"
#include "Unit.h"
#include "raylibEx.h"
#include <raylib.h>

namespace CameraControl
{
    // Edge pan
    float constexpr EDGE_PAN_TRIGGER_WIDTH{3.5F * TextureData::TILE_SIZE};
    int constexpr PAN_BOOST_FACTOR{4};
    float constexpr PAN_TICK{0.1F}; // time in seconds to next pan move step

    void edgePan(Camera2D* camera, Vector2 cursorWorldPosition, bool isMouseControlled, RectangleExI const& mapArea, RectangleEx const& panelMap);
    void centerOnHero(Camera2D* camera, Unit const& unit);
}

#endif
