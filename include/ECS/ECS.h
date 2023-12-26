#ifndef ECS_H_20231215005456
#define ECS_H_20231215005456

#include "ComponentManager.h"
#include "ContiguousContainer.h"
#include "EntityId.h"
#include "EntityManager.h"
#include "Id.h"
#include "Signature.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>

namespace snd
{
    using ComponentTypeId = Id;

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
            ComponentTypeId componentTypeId = Component<ComponentType>::getId();

            componentManagers_[componentTypeId] = std::make_shared<ComponentManager<ComponentType>>();
        }

        template <typename ComponentType>
        static void assignComponent(EntityId entity, ComponentType component)
        {
            // get component type id
            ComponentTypeId componentTypeId = Component<ComponentType>::getId();

            // retrieve ComponentManager
            auto componentManager{std::static_pointer_cast<ComponentManager<ComponentType>>(
                componentManagers_.at(componentTypeId))};

            // request entity signature
            Signature oldSignature{entityManager_.requestSignature(entity)};

            // set type in entity signature
            entityManager_.setSignature(entity, componentTypeId);

            // request entity signature
            Signature newSignature{entityManager_.requestSignature(entity)};

            // assign component to entity
            componentManager->assignTo(newSignature, entity, component);

            // update other componentManagers
            updateComponentManagers(oldSignature, newSignature, entity);
        }

        template <typename ComponentType>
        static void removeComponent(EntityId entity)
        {
            // get type id
            ComponentTypeId componentTypeId = Component<ComponentType>::getId();

            // retrieve ComponentManager
            std::shared_ptr<ComponentManager<ComponentType>> componentManager{std::static_pointer_cast<ComponentManager<ComponentType>>(
                componentManagers_.at(componentTypeId))};

            // request entity signature
            Signature& oldSignature{entityManager_.requestSignature(entity)};

            // reset type in entity signature
            entityManager_.resetSignature(entity, componentTypeId);

            // request entity signature
            Signature& newSignature{entityManager_.requestSignature(entity)};

            // remove component from entity
            componentManager->removeFrom(oldSignature, entity);

            // update other componentManagers
            updateComponentManagers(oldSignature, newSignature, entity);
        }

        template <typename ComponentType>
        static ComponentType* retrieveComponent(EntityId entity)
        {
            // get component type id
            ComponentTypeId componentTypeId = Component<ComponentType>::getId();

            // request entity signature
            Signature& signature{entityManager_.requestSignature(entity)};

            // return component
            return std::static_pointer_cast<ComponentManager<ComponentType>>(
                       componentManagers_.at(componentTypeId))
                ->retrieveFrom(signature, entity);
        }
        // ============================

        // Logic
        // ============================
        // ============================

    private:
        static inline EntityManager entityManager_;
        static inline std::unordered_map<ComponentTypeId, std::shared_ptr<BaseComponentManager>> componentManagers_;

        // Components
        // ============================
        static void updateComponentManagers(const Signature& oldSignature, const Signature& newSignature, EntityId entity)
        {
            // check if signature changed
            // if (oldSignature == newSignature)
            if (oldSignature == newSignature)
                return;

            // get unchanged component types
            Signature unchangedTypes{oldSignature & newSignature};

            // check if there are types to update
            if (unchangedTypes == 0)
                return;

            std::cout << "Update component managers of unchanged signature parts: " << unchangedTypes << "\n";

            for (auto& manager : componentManagers_)
            {
                // get type id
                ComponentTypeId componentTypeId = manager.first;

                // check if manager matches unchanged types
                if (!unchangedTypes.test(componentTypeId))
                {
                    continue;
                }

                // update containers
                manager.second->updateSignature(oldSignature, newSignature, entity);
            }
        }
        // ============================
    };
}

#endif
