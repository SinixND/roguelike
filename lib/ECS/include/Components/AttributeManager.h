#ifndef ATTRIBUTEMANAGER_H_20231212234818
#define ATTRIBUTEMANAGER_H_20231212234818

#include "Entity.h"
#include "Id.h"
#include "IdManager.h"
#include <functional>
#include <map>
#include <vector>

typedef size_t Index;

namespace snd
{
    template <typename AttributeType>
    class AttributeManager
    {
    public:
        // Add a attribute to an entity
        void assign(Entity entity, AttributeType attribute)
        {
            Index attributeIndex = attributes_.size();

            attributes_.push_back(attribute);
            entityToAttribute_.insert(std::make_pair(entity, attributeIndex));
            attributeToEntity_.insert(std::make_pair(attributeIndex, entity));
        };

        // Access a attribute from a specific entity
        AttributeType& get(Entity entity)
        {
            return &attributes_[entityToAttribute_[entity]];
        };

        // Remove a attribute from an entity
        void remove(Entity entity)
        {
            if (!entityToAttribute_.find(entity)->first)
            {
                return;
            };

            Index removedAttributeIndex = entityToAttribute_[entity];

            // replace with last attribute before popping if more than one element to keep elements contiguous
            if (attributes_.size() > 1)
            {
                Index lastAttributeIndex = attributes_.size() - 1;
                Entity keptEntity = attributeToEntity_[lastAttributeIndex];

                // swap attributes (by index) so last entry can be popped
                attributes_[removedAttributeIndex] = attributes_[lastAttributeIndex];
                // update mapping (by id)
                entityToAttribute_[keptEntity] = removedAttributeIndex;
            }

            // remove removed entity from mapping
            entityToAttribute_.erase(entity);
            // remove removed attribute from mapping
            attributeToEntity_.erase(removedAttributeIndex);

            attributes_.pop_back();
        };

        // 4. Iterate over all items
        void iterateAll(std::function<void(AttributeType attribute)> lambda)
        {
            for (auto& attribute : attributes_)
            {
                lambda(attribute);
            };
        };

        Id getAttributeTypeId() { return attributeTypeId_; };

        AttributeManager()
        {
            IdManager* idManager{IdManager::getInstance()};

            attributeTypeId_ = idManager->requestId();
        };

    private:
        static Id attributeTypeId_;

        std::vector<AttributeType> attributes_;     // vector index is used as attribute id
        std::map<Entity, Index> entityToAttribute_; // entity is used to identify attribute, because attribute cant exist alone
        std::map<Index, Entity> attributeToEntity_; // store a attribute to entity mapping for removing
    };
}
#endif

template <typename AttributeType>
snd::Id snd::AttributeManager<AttributeType>::attributeTypeId_{0};
