#ifndef SYSTEM_H_20231217204502
#define SYSTEM_H_20231217204502

#include "Entity.h"
#include <set>

namespace snd
{
    class System
    {
    public:
        void addEntity(Entity entity)
        {
            entities_.insert(entity);
        };

        void removeEntity(Entity entity)
        {
            entities_.erase(entity);
        };

        virtual ~System();

    protected:
        std::set<Entity> entities_;
    };
}

#endif