#ifndef _20240211172120
#define _20240211172120

#include "IEntity.h"
#include "Position.h"
#include "RenderData.h"

class GameObject : public IEntity
{
public:
    Position position_{};
    RenderData renderData_{};
};

#endif