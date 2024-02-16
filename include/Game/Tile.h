#ifndef _20240215012942
#define _20240215012942

#include "GameObject.h"
#include "Position.h"
#include "RenderData.h"

class Tile : public GameObject
{
public:
    bool isSolid() { return isSolid_; };
    void setIsSolid(bool isSolid) { isSolid_ = isSolid; };

private:
    bool isSolid_{};
};
#endif