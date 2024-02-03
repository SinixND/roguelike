#ifndef ECS_H_20231215005456
#define ECS_H_20231215005456

#include "Component.h"
#include "ComponentManager.h"
#include "ComponentTypeId.h"
#include "EntityId.h"
#include "EntityManager.h"
#include "Signature.h"
#include "SparseSet.h"
#include "System.h"
#include "SystemManager.h"
#include <memory>

namespace snd
{
    class ECS
    {
    public:
        // Entities
        // ============================
        EntityId createEntity()
        {
            return entityManager_.request();
        }

        void removeEntity(EntityId entityId)
        {
            removeAllComponents(entityId);
            entityManager_.remove(entityId);
        }

        Signature& requestEntitySignature(EntityId entityId)
        {
            return entityManager_.getSignature(entityId);
        }

        template <typename ComponentType>
        EntityId getFirstEntityWith()
        {
            return componentManager_.getFirstEntity<ComponentType>();
        }

        template <typename ComponentType>
        std::unordered_set<EntityId>* getAllEntitiesWith()
        {
            return componentManager_.getAllEntities<ComponentType>();
        }
        // ============================

        // Components
        // ============================
        template <typename ComponentType, typename... Args>
        void assignComponent(EntityId entityId, Args&&... args)
        {
            // Assign component to entity
            ComponentType component{std::forward<Args>(args)...};
            componentManager_.assignTo(entityId, component);

            // get component type id
            ComponentTypeId componentTypeId{Component<ComponentType>::getId()};

            // Update entity signature
            entityManager_.setComponent(entityId, componentTypeId);

            // notify systems about added component
            notifyAdd(entityId, componentTypeId);
        }

        template <typename ComponentType>
        void removeComponent(EntityId entityId)
        {
            // get component type id
            ComponentTypeId componentTypeId{Component<ComponentType>::getId()};

            // Remove component from entity
            componentManager_.removeFrom<ComponentType>(entityId);

            // notify systems about removed component
            notifyRemove(entityId, componentTypeId);

            // Update entity signature
            entityManager_.resetComponent(entityId, componentTypeId);
        }

        void removeAllComponents(EntityId entityId)
        {
            // Get containers
            auto containers{componentManager_.getAllContainers()};

            for (auto& [componentTypeId, container] : *containers)
            {
                // Remove component from entity
                container->erase(entityId);

                // Notify systems about removed component
                notifyRemove(entityId, componentTypeId);

                // Update entity signature
                entityManager_.resetComponent(entityId, componentTypeId);
            }
        }

        template <typename ComponentType>
        ComponentType* getComponent(EntityId entityId)
        {
            // Return component
            return componentManager_.getFrom<ComponentType>(entityId);
        }

        template <typename ComponentType>
        void toggleComponent(EntityId entityId)
        {
            if (getComponent<ComponentType>(entityId))
            {
                removeComponent<ComponentType>(entityId);
            }
            else
            {
                assignComponent<ComponentType>(entityId);
            }
        }

        template <typename ComponentType>
        std::vector<ComponentType>* getAllComponents()
        {
            return componentManager_.getAllComponents<ComponentType>();
        }
        // ============================

        // Systems
        // ============================
        template <typename SystemType>
        auto registerSystem()
        {
            auto system{std::make_shared<SystemType>(this)};

            systems_.push_back(system);
            return system;
        }
        // ============================

    private:
        EntityManager entityManager_;
        ComponentManager componentManager_;
        std::vector<std::shared_ptr<ISystem>> systems_;

    private:
        // Systems
        // ============================
        void notifyAdd(EntityId entityId, ComponentTypeId componentTypeId)
        {
            for (const auto& system : systems_)
            {
                // get system signature
                auto systemSignature{system->getSignature()};

                // check if component type is relevant to system
                if (!systemSignature.test(componentTypeId)) continue;

                // check if entity signature is relevant to system
                if ((entityManager_.getSignature(entityId) & systemSignature) != systemSignature) continue;

                // register entity
                system->registerEntity(entityId);
            }
        }

        void notifyRemove(EntityId entityId, ComponentTypeId componentTypeId)
        {
            for (const auto& system : systems_)
            {
                // get system signature
                auto systemSignature{system->getSignature()};

                // check if component type is relevant to system
                if (!systemSignature.test(componentTypeId)) continue;

                // check if entity signature is relevant to system
                if ((entityManager_.getSignature(entityId) & systemSignature) != systemSignature) continue;

                // deregister entity
                system->deregisterEntity(entityId);
            }
        }
        // ============================
    };
}

#endif
