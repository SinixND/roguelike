#ifndef ENITITYMANAGER_H_20231213001142
#define ENITITYMANAGER_H_20231213001142

#include "Entity.h"
#include <unordered_map>
#include <vector>

namespace snd
{
    class EntityManager
    {
    public:
        Entity create();
        void remove(Entity entity);

    private:
        std::unordered_map<Entity, std::vector<bool>> entities_{};
    };
}
#endif