#ifndef SYSTEM_H_20231226184152
#define SYSTEM_H_20231226184152

#include "Component.h"
#include "ComponentManager.h"
#include "EntityId.h"
#include "Signature.h"
#include "SystemId.h"
#include <iostream>
#include <unordered_set>

class ECS;

namespace snd
{
    class BaseSystem
    {
    public:
        virtual void action(EntityId) = 0;

        void execute()
        {
            for (auto& entity : entities_)
            {
                action(entity);
            }
        }

        void registerEntity(EntityId entity)
        {
            entities_.insert(entity);
        }

        void deregisterEntity(EntityId entity)
        {
            entities_.erase(entity);
        }

        Signature* getSignature()
        {
            return &signature_;
        }

        virtual ~BaseSystem() = default;

    protected:
        std::unordered_set<EntityId> entities_;
        Signature signature_;
    };

    template <typename ComponentType, typename... ComponentTypes>
    class System : public BaseSystem
    {
    public:
        virtual void action(EntityId) = 0;

        System(ComponentManager& componentManager)
            : componentManager_(componentManager)
        {
            registerComponentTypes<ComponentType, ComponentTypes...>();
        }

    protected:
        [[maybe_unused]] ComponentManager& componentManager_;

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
