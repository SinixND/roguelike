#ifndef COMPONENTMANAGER_H_20231212234818
#define COMPONENTMANAGER_H_20231212234818

#include "ContiguousContainer.h"
#include "Entity.h"
#include "Id.h"
#include "IdManager.h"
#include <cassert>
#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

typedef size_t Index;

namespace snd
{
    class BaseComponentManager
    {
    public:
        BaseComponentManager() = default;
        virtual ~BaseComponentManager() = default;
        BaseComponentManager(const BaseComponentManager&) = default;
        BaseComponentManager& operator=(const BaseComponentManager&) = default;
        BaseComponentManager(BaseComponentManager&&) = default;
        BaseComponentManager& operator=(BaseComponentManager&&) = default;
    };

    template <typename ComponentType>
    class ComponentManager : public BaseComponentManager
    {
    public:
        Id getComponentTypeId() { return componentTypeId_; };

        // Add a component to an entity
        void assignTo(Entity entity, ComponentType component)
        {
            assignedComponents_.addElement(entity, component);
            componentMaskEntities_.addElement(componentTypeId_, std::set<Entity>{entity});
        };

        // Access a component from a specific entity
        ComponentType& retrieveFrom(Entity entity)
        {
            return assignedComponents_.retrieveElement(entity);
        };

        std::set<Entity>& retrieveMask()
        {
            return componentMaskEntities_.retrieveElement(componentTypeId_);
        }

        // Remove a component from an entity
        void removeFrom(Entity entity)
        {
            assignedComponents_.removeElement(entity);
            componentMaskEntities_.retrieveElement(componentTypeId_).erase(entity);
        };

        // 4. Iterate over all items
        // void iterateAll(std::function<void(ComponentType component)> lambda)
        //{
        // assignedComponents_.iterateAllElements(lambda);
        //};

        ComponentManager()
        {
            IdManager* idManager{IdManager::getInstance()};

            componentTypeId_ = idManager->requestId();
        };

    private:
        Id componentTypeId_;

        ContiguousContainer<Id, std::set<Entity>> componentMaskEntities_;

        ContiguousContainer<Id, ComponentType> assignedComponents_;
    };

}

#endif