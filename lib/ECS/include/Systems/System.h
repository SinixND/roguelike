#ifndef SYSTEM_H_20231217204502
#define SYSTEM_H_20231217204502

#include "EntityId.h"
#include <set>

namespace snd
{
    class System
    {
    public:
        void addEntity(EntityId entity)
        {
            entities_.insert(entity);
        };

        void removeEntity(EntityId entity)
        {
            entities_.erase(entity);
        };

        virtual ~System();

    protected:
        std::set<EntityId> entities_;
    };
}

#endif