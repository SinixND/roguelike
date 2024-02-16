#ifndef _20240215004424
#define _20240215004424

#include "Id.h"
#include "IdManager.h"

static snd::IdManager idManager{};

class Entity
{
public:
    snd::Id id_{idManager.requestId()};

    virtual ~Entity() { idManager.suspendId(id_); }
};

#endif