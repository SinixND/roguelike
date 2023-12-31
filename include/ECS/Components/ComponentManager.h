#ifndef COMPONENTMANAGER_H_20240101214521
#define COMPONENTMANAGER_H_20240101214521

#include "Component.h"
#include "ComponentTypeId.h"
#include "ContiguousMap.h"
#include "EntityId.h"
#include <memory>
#include <unordered_map>

namespace snd
{
    class ComponentManager
    {
    public:
        // Add component to entity
        template <typename ComponentType>
        void assignTo(EntityId entityId, const ComponentType& component)
        {
            // Check if container exists
            if (!componentContainersByTypeId_.contains(getId<ComponentType>()))
            {
                registerComponentType<ComponentType>();
            }

            // Add component
            getComponentContainer<ComponentType>()->insert(entityId, component);
        }

        // Remove a component from an entity
        template <typename ComponentType>
        void removeFrom(EntityId entityId)
        {
            // Check if entity exists
            if (!testEntity<ComponentType>(entityId))
                return;

            // Remove component
            getComponentContainer<ComponentType>()->erase(entityId);
        }

        // Access a component from a specific entity
        template <typename ComponentType>
        ComponentType* retrieveFrom(EntityId entityId)
        {
            // Check if entity exists
            if (!testEntity<ComponentType>(entityId))
                return nullptr;

            // Return component
            return getComponentContainer<ComponentType>()->get(entityId);
        }

        // Access all components
        template <typename ComponentType>
        std::vector<ComponentType>* retrieveAll()
        {
            // Check if container exists
            if (!testContainer<ComponentType>()) return nullptr;

            // Return vector of components
            return getComponentContainer<ComponentType>()->getAll();
        }

    private:
        std::unordered_map<ComponentTypeId, std::shared_ptr<BaseContiguousMap>> componentContainersByTypeId_{};

    private:
        // Check if entity exists in container
        template <typename ComponentType>
        bool testEntity(EntityId entityId)
        {
            if (!testContainer<ComponentType>()) return false;

            return getComponentContainer<ComponentType>()->test(entityId);
        }

        // Check if container exists
        template <typename ComponentType>
        bool testContainer()
        {
            return componentContainersByTypeId_.contains(getId<ComponentType>());
        }

        // Get component type id
        template <typename ComponentType>
        ComponentTypeId getId() { return Component<ComponentType>::getId(); }

        // Register component type
        template <typename ComponentType>
        void registerComponentType()
        {
            componentContainersByTypeId_[getId<ComponentType>()] = std::make_shared<ContiguousMap<EntityId, ComponentType>>();
        }

        // Return specialized container pointer
        template <typename ComponentType>
        std::shared_ptr<ContiguousMap<EntityId, ComponentType>> getComponentContainer()
        {
            return std::static_pointer_cast<ContiguousMap<EntityId, ComponentType>>(componentContainersByTypeId_[getId<ComponentType>()]);
        }
    };
}

#endif
