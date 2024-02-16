#ifndef _20240211172120
#define _20240211172120

#include "Entity.h"
#include "Position.h"
#include "RenderData.h"

class GameObject : public Entity
{
public:
    Position& position() { return position_; };
    void setPosition(Position position) { position_ = position; };

    RenderData& renderData() { return renderData_; };
    void setRenderData(RenderData renderData) { renderData_ = renderData; };

private:
    Position position_{};
    RenderData renderData_{};
};

#endif