#ifndef ENTITY_H_20240208012031
#define ENTITY_H_20240208012031

#include "ECS.h"
#include "System.h"

class IEntity
{
protected:
    snd::ECS& ecs_;

public:
    snd::EntityId id_;

public:
    IEntity(snd::ECS& ecs)
        : ecs_(ecs)
        , id_(ecs_.createEntity()){};

    virtual ~IEntity() { ecs_.removeEntity(id_); };
};

#endif