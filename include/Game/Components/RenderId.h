#ifndef RENDERID_H_20240208015954
#define RENDERID_H_20240208015954

#include "Component.h"
#include "RuntimeDatabase.h"

struct RenderId
    : public snd::Component<RenderId>
{
    RenderType type;
};

#endif