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
#include <unordered_map>

namespace snd
{
    class ECS
    {
    public:
        // ECS
        static void init()
        {
        }

        // Entities
        static EntityId createEntity()
        {
            return entityManager_.create();
        }

        static void removeEntity(EntityId entity)
        {
            entityManager_.remove(entity);
        }

        // Components
        template <typename ComponentType>
        static void registerComponent()
        {
            // get type id
            Id componentTypeId = Component<ComponentType>::getId();

            componentManagers_[componentTypeId] = std::make_shared<ComponentManager<ComponentType>>();
        }
        /*
                template <typename ComponentType>
                static void assignComponent(EntityId entity, ComponentType component)
                {
                    // get type id
                    Id componentTypeId = Component<ComponentType>::getId();

                    // check if manager exists
                    if (!componentManagers_.count(componentTypeId))
                    {
                        // add non-existing manager
                        componentManagers_.insert(std::make_pair(componentTypeId, std::make_shared<ComponentManager<ComponentType>>()));
                    }

                    // retrieve ComponentManager
                    auto componentManager{std::static_pointer_cast<ComponentManager<ComponentType>>(
                        componentManagers_.at(componentTypeId))};

                    // assign component to entity
                    componentManager->assignTo(entity, component);

                    // update entityMask
                    if (!entitySignature_.tryElement(entity))
                    {
                        // add new Mask + component
                        entitySignature_.addElement(entity, std::set<Id>{Component<ComponentType>::getId()});

                        return;
                    }

                    // add component
                    entitySignature_.retrieveElement(entity).insert(Component<ComponentType>::getId());
                }

                template <typename ComponentType>
                static ComponentType& retrieveComponent(EntityId entity)
                {
                    // get type id
                    Id componentTypeId = Component<ComponentType>::getId();

                    // assert if manager exists (else no component can be returned)
                    assert(componentManagers_.count(componentTypeId) && "NO ASSIGNED COMPONENT FOUND!");

                    // return component
                    return std::static_pointer_cast<ComponentManager<ComponentType>>(
                               componentManagers_.at(componentTypeId))
                        ->retrieveFrom(entity);
                }

                static std::set<Id>& retrieveEntityMask(EntityId entity)
                {
                    return entitySignature_.retrieveElement(entity);
                }

                template <typename ComponentType>
                static std::set<EntityId>& retrieveComponentMask()
                {
                    // get type id
                    Id componentTypeId = Component<ComponentType>::getId();

                    // retrieve ComponentManager
                    auto componentManager{std::static_pointer_cast<ComponentManager<ComponentType>>(
                        componentManagers_.at(componentTypeId))};

                    return componentManager->retrieveAssociatedEntities();
                }

                template <typename ComponentType>
                static void removeComponent(EntityId entity)
                {
                    // get type id
                    Id componentTypeId = Component<ComponentType>::getId();

                    // check if manager exists
                    if (!componentManagers_.count(componentTypeId))
                    {
                        // no removable component exists
                        return;
                    }

                    // retrieve ComponentManager
                    auto componentManager{std::static_pointer_cast<ComponentManager<ComponentType>>(
                        componentManagers_.at(componentTypeId))};

                    // update entityMask
                    entitySignature_.retrieveElement(entity).erase(Component<ComponentType>::getId());

                    // remove component from entity
                    componentManager->removeFrom(entity);
                }
        */

        // template <typename ComponentType>
        // void iterateAll(std::function<void(ComponentType component)> lambda)
        //{
        // Id componentTypeId{typeid(ComponentType).name()};
        // if (!componentManagers_.count(componentTypeId))
        //{
        // return;
        //}
        // std::static_pointer_cast<ComponentManager<ComponentType>>(
        // componentManagers_.at(componentTypeId))
        //->iterateAll(lambda);
        //}

        // Logic

    private:
        static inline EntityManager entityManager_;
        static inline std::unordered_map<Id, std::shared_ptr<BaseComponentManager>> componentManagers_;
    };
}

#endif