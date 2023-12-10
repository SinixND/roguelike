#ifndef ENTITYMANAGER_H_20231210202055
#define ENTITYMANAGER_H_20231210202055

#include "Entity.h"
#include <map>
#include <mutex>

#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace snd
{
    class EntityManager
    {
    public:
        static Entity createEntity();
        static void removeEntity(Entity entity);

        // Singleton attributes
        //---------------------------------
        static EntityManager* getInstance();
        //---------------------------------

    private:
        static int lastEntity_;

        // Singleton attributes
        //---------------------------------
        static EntityManager* singleton_;
        static std::mutex mutex_;
        EntityManager(){};
        ~EntityManager(){};
        DISALLOW_COPY_AND_ASSIGN(EntityManager)
        //---------------------------------
    };

}

#endif