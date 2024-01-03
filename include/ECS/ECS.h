#ifndef ECS_H_20231215005456
#define ECS_H_20231215005456

#include "Component.h"
#include "ComponentManager.h"
#include "ComponentTypeId.h"
#include "ContiguousMap.h"
#include "EntityId.h"
#include "EntityManager.h"
#include "Signature.h"
#include "System.h"
#include "SystemManager.h"
#include <iostream>
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

        void removeEntity(EntityId entity)
        {
            entityManager_.remove(entity);
        }

        Signature* requestEntitySignature(EntityId entity)
        {
            return entityManager_.getSignature(entity);
        }
        // ============================

        // Components
        // ============================
        template <typename ComponentType>
        void assignComponent(EntityId entity, const ComponentType& component)
        {
            // Assign component to entity
            componentManager_.assignTo(entity, component);

            // get component type id
            ComponentTypeId componentTypeId{
                Component<ComponentType>::getId()};

            // Update entity signature
            entityManager_.setComponent(entity, componentTypeId);

            // notify systems about added component
            // (entity can only become relevant, not obsolete)
            notifyAdd(entity, componentTypeId);
        }

        template <typename ComponentType>
        void removeComponent(EntityId entity)
        {
            // Remove component from entity
            componentManager_.removeFrom<ComponentType>(entity);

            // get component type id
            ComponentTypeId componentTypeId{
                Component<ComponentType>::getId()};

            // Update entity signature
            entityManager_.resetComponent(entity, componentTypeId);

            // notify systems about removed component
            // (entity can only become obsolete, not relevant)
            notifyRemove(entity, componentTypeId);
        }

        template <typename ComponentType>
        ComponentType* retrieveComponent(EntityId entity)
        {
            // Return component
            return componentManager_.retrieveFrom<ComponentType>(entity);
        }

        template <typename ComponentType>
        std::vector<ComponentType>* retrieveAllComponents()
        {
            return componentManager_.retrieveAll<ComponentType>();
        }
        // ============================

        // Systems
        // ============================
        template <typename SystemType>
        std::shared_ptr<SystemType> registerSystem()
        {
            auto system{
                std::make_shared<SystemType>(componentManager_)};

            systems_.push_back(system);
            return system;
        }
        // ============================

    private:
        EntityManager entityManager_;
        ComponentManager componentManager_;
        std::vector<std::shared_ptr<BaseSystem>> systems_;
    };
}

#endif
