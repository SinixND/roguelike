#ifndef SYSTEM_H_20231217204502
#define SYSTEM_H_20231217204502

#include "EntityId.h"
#include "Id.h"
#include <initializer_list>
#include <set>

namespace snd
{
    class System
    {
    public:
        void setRequiredComponents(Id componentTypeId)
        {
            requiredComponentTypeIds_.insert(componentTypeId);
        };

        // provide required component types
        std::set<Id>& retrieveRequiredComponentTypIds()
        {
            return requiredComponentTypeIds_;
        };

        // add entity to processed entities
        void registerEntity(Entity entity)
        {
            processedEntities_.insert(entity);
        };

        // remove entity from processed entities
        void deRegisterEntity(Entity entity)
        {
            processedEntities_.erase(entity);
        };

        // perform action on all processed entities
        void execute(int deltaTime = 0)
        {
            for (auto entity : processedEntities_)
            {
                action(entity);
            }
        };

        virtual ~System() = default;
        System(const System&) = default;
        System& operator=(const System&) = default;
        System(System&&) = default;
        System& operator=(System&&) = default;

    protected:
        std::set<Entity> processedEntities_;
        std::set<Id> requiredComponentTypeIds_;

        // system specific action executed on one processed entity
        virtual void action(Entity entity){};
    };
}

#endif
