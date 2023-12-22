#ifndef SYSTEM_H_20231217204502
#define SYSTEM_H_20231217204502

#include "EntityId.h"
#include "Id.h"
#include "Signature.h"
#include <unordered_set>

namespace snd
{
    class System
    {
    public:
        void setSystemSignature(Id componentTypeId)
        {
            systemSignature_.set(componentTypeId);
        };

        // provide required component types
        Signature& retrieveSystemSignature()
        {
            return systemSignature_;
        };

        // add entity to processed entities
        void syncRegisterComponents(Signature signature)
        {
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
        std::unordered_set<Entity> processedEntities_;
        Signature systemSignature_;

        // system specific action executed on one processed entity
        virtual void action(Entity entity){};
    };
}

#endif
