#ifndef ENTITYMANAGER_H_20231220225105
#define ENTITYMANAGER_H_20231220225105

#include "ContiguousContainer.h"
#include "EntityId.h"
#include "IdManager.h"
#include "Signature.h"
#include <unordered_map>
#include <utility>

namespace snd
{
    class EntityManager
    {
    public:
        EntityId create()
        {
            Id newId{entityIdManager_.requestId()};

            entity_to_signature_.insert(std::make_pair(newId, Signature()));

            return newId;
        };

        void remove(EntityId entityId)
        {
            entityIdManager_.suspendId(entityId);
            entity_to_signature_.erase(entityId);
        };

        void setSignature(EntityId entity, Id componentType)
        {
            entity_to_signature_.at(entity).set(componentType);
        };

        void resetSignature(EntityId entity, Id componentType)
        {
            entity_to_signature_.at(entity).reset(componentType);
        };

        Signature& requestSignature(EntityId entity)
        {
            return entity_to_signature_.at(entity);
        };

    private:
        IdManager entityIdManager_;
        std::unordered_map<EntityId, Signature> entity_to_signature_;
    };
}

#endif