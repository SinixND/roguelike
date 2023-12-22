#ifndef ECS_H_20231215005456
#define ECS_H_20231215005456

#include "ComponentManager.h"
#include "ContiguousContainer.h"
#include "Entities/EntityManager.h"
#include "EntityId.h"
#include "EntityManager.h"
#include "Id.h"
#include "Signature.h"
#include <cassert>
#include <memory>
#include <set>
#include <iostream>
#include <unordered_map>

namespace snd
{
    class ECS
    {
    public:
        // ECS
        // ============================
        static void init()
        {
        }
        // ============================

        // Entities
        // ============================
        static EntityId createEntity()
        {
            return entityManager_.create();
        }

        static void removeEntity(EntityId entity)
        {
            entityManager_.remove(entity);
        }
        // ============================

        // Components
        // ============================
        template <typename ComponentType>
        static void registerComponent()
        {
            // get type id
            Id componentTypeId = Component<ComponentType>::getId();

            componentManagers_[componentTypeId] = std::make_shared<ComponentManager<ComponentType>>();
        }

        template <typename ComponentType>
        static void assignComponent(EntityId entity, ComponentType component)
        {
            // get component type id
            Id componentTypeId = Component<ComponentType>::getId();

            // set entity signature
            entityManager_.setSignature(entity, componentTypeId);

            // request entity signature
            Signature newSignature{entityManager_.requestSignature(entity)};

            // retrieve ComponentManager
            auto componentManager{std::static_pointer_cast<ComponentManager<ComponentType>>(
                componentManagers_.at(componentTypeId))};

            // assign component to entity
            componentManager->assignTo(newSignature, entity, component);
        }

        template <typename ComponentType>
        static ComponentType* retrieveComponent(EntityId entity)
        {
            // get component type id
            Id componentTypeId = Component<ComponentType>::getId();

            // request entity signature
            Signature& signature{entityManager_.requestSignature(entity)};

            // return component
            return std::static_pointer_cast<ComponentManager<ComponentType>>(
                       componentManagers_.at(componentTypeId))
                ->retrieveFrom(signature, entity);
        }

        template <typename ComponentType>
        static void removeComponent(EntityId entity)
        {
            // get type id
            Id componentTypeId = Component<ComponentType>::getId();

            // request entity signature
            Signature& oldSignature{entityManager_.requestSignature(entity)};

            // retrieve ComponentManager
            std::shared_ptr<ComponentManager<ComponentType>> componentManager{std::static_pointer_cast<ComponentManager<ComponentType>>(
                componentManagers_.at(componentTypeId))};

            // remove component from entity
            componentManager->removeFrom(oldSignature, entity);

            // set entity signature
            entityManager_.resetSignature(entity, componentTypeId);
        }
        // ============================

        // Logic
        // ============================
        // ============================

    private:
        static inline EntityManager entityManager_;
        static inline std::unordered_map<Id, std::shared_ptr<BaseComponentManager>> componentManagers_;
    };
}

#endif
