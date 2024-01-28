#ifndef SYSTEM_H_20231226184152
#define SYSTEM_H_20231226184152

#include "Component.h"
#include "EntityId.h"
#include "Signature.h"
#include <unordered_set>

namespace snd
{
    class ECS;

    class ISystem
    {
    public:
        virtual void action(EntityId) = 0;

        void execute()
        {
            for (auto entityId : entities_)
            {
                action(entityId);
            }
        }

        void registerEntity(EntityId entityId)
        {
            entities_.insert(entityId);
        }

        void deregisterEntity(EntityId entityId)
        {
            entities_.erase(entityId);
        }

        Signature& getSignature()
        {
            return signature_;
        }

        virtual ~ISystem() = default;

    protected:
        std::unordered_set<EntityId> entities_;
        Signature signature_;
    };

    template <typename ComponentType, typename... ComponentTypes>
    class System
        : public ISystem
    {
    public:
        virtual void action(EntityId) = 0;
        void cleanup();

        System(ECS* ecs)
            : ecs_(ecs)
        {
            registerComponentTypes<ComponentType, ComponentTypes...>();
        }

    protected:
        ECS* ecs_;

    protected:
        template <typename LastType>
        void registerComponentTypes()
        {
            signature_.set(Component<LastType>::getId());
        }

        template <typename FirstType, typename SecondType, typename... RemainingTypes>
        void registerComponentTypes()
        {
            registerComponentTypes<FirstType>();
            registerComponentTypes<SecondType, RemainingTypes...>();
        }
    };
}

#endif
