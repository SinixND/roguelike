#ifndef COMPONENTMANAGER_H_20231212234818
#define COMPONENTMANAGER_H_20231212234818

#include "Component.h"
#include "ContiguousContainer.h"
#include "EntityId.h"
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
        // Add a component to an entity
        void assignTo(EntityId entity, ComponentType component)
        {
            components_.addElement(entity, component);

            associatedEntities_.insert(entity);
        };

        // Access a component from a specific entity
        ComponentType& retrieveFrom(EntityId entity)
        {
            return components_.retrieveElement(entity);
        };

        // Remove a component from an entity
        void removeFrom(EntityId entity)
        {
            components_.removeElement(entity);

            associatedEntities_.erase(entity);
        };

        std::vector<ComponentType>& retrieveComponents()
        {
            return components_.retrieveAllElements();
        };

        std::set<EntityId>& retrieveAssociatedEntities()
        {
            return associatedEntities_;
        }

        // 4. Iterate over all items
        // void iterateAll(std::function<void(ComponentType component)> lambda)
        //{
        // assignedComponents_.iterateAllElements(lambda);
        //};

    private:
        ContiguousContainer<Id, ComponentType> components_;

        std::set<EntityId> associatedEntities_;
    };

}

#endif
