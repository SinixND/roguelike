#ifndef _20240218231106
#define _20240218231106

#include "Render.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <raylib.h>

class Graphic
{
public:
    RenderID renderID() { return renderID_; };
    void setRenderID(RenderID renderID) { renderID_ = renderID; };

    void render(const Vector2Int& position)
    {
        renderAction(renderID_, position);
    };

private:
    RenderID renderID_{};
};

#endif