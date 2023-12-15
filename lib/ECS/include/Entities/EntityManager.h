#ifndef ENITITYMANAGER_H_20231213001142
#define ENITITYMANAGER_H_20231213001142

#include "Entity.h"
#include <set>

namespace snd
{
    class EntityManager
    {
    public:
        Entity create();
        void remove(Entity entity);

    private:
        std::set<Entity> entities_{};
    };
}
#endif