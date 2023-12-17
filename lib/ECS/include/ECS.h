#ifndef ECS_H_20231215005456
#define ECS_H_20231215005456

#include "ComponentManager.h"
#include "EntityManager.h"
#include "Id.h"
#include <any>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>

namespace snd
{
    class ECS
    {
    public:
        // ECS
        static void init()
        {
            entityManager = std::make_unique<EntityManager>();
        }

        // Entities
        static Entity createEntity()
        {
            return entityManager->create();
        }

        static void removeEntity(Entity entity)
        {
            entityManager->remove(entity);
        }

        // Components
        template <typename ComponentType>
        static void assignComponent(Entity entity, ComponentType component)
        {
            std::string componentTypeId{typeid(ComponentType).name()};

            if (!componentManagers_.count(componentTypeId))
            {
                componentManagers_.insert(std::make_pair(componentTypeId, std::make_shared<ComponentManager<ComponentType>>()));
            }

            std::static_pointer_cast<ComponentManager<ComponentType>>(
                componentManagers_.at(componentTypeId))
                ->assignTo(entity, component);
        }

        template <typename ComponentType>
        static ComponentType& retrieveComponent(Entity entity)
        {
            std::string componentTypeId{typeid(ComponentType).name()};

            assert(componentManagers_.count(componentTypeId) && "NO ASSIGNED COMPONENT FOUND!");

            return std::static_pointer_cast<ComponentManager<ComponentType>>(
                       componentManagers_.at(componentTypeId))
                ->retrieveFrom(entity);
        }

        template <typename ComponentType>
        static void removeComponent(Entity entity)
        {
            std::string componentTypeId{typeid(ComponentType).name()};

            if (!componentManagers_.count(componentTypeId))
            {
                return;
            }

            std::static_pointer_cast<ComponentManager<ComponentType>>(
                componentManagers_.at(componentTypeId))
                ->removeFrom(entity);
        }

        // template <typename ComponentType>
        // void iterateAll(std::function<void(ComponentType component)> lambda)
        //{
        // std::string componentTypeId{typeid(ComponentType).name()};

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
        // Managers
        static std::unique_ptr<EntityManager> entityManager;

        static std::map<std::string, std::shared_ptr<BaseComponentManager>> componentManagers_;

        // Container<std::shared_ptr<System>> systems;
    };
}

#endif