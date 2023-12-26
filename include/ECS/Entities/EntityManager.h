#ifndef ENTITYMANAGER_H_20231220225105
#define ENTITYMANAGER_H_20231220225105

#include "ContiguousContainer.h"
#include "EntityId.h"
#include "IdManager.h"
#include "Signature.h"
#include <iostream>
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

            entityToSignature_.insert(std::make_pair(newId, Signature()));

            return newId;
        };

        void remove(EntityId entityId)
        {
            entityIdManager_.suspendId(entityId);
            entityToSignature_.erase(entityId);
        };

        void setSignature(EntityId entity, Id componentType)
        {
            entityToSignature_.at(entity).set(componentType);
            std::cout << "Set EntitySig: " << entityToSignature_.at(entity) << "\n";
        };

        void resetSignature(EntityId entity, Id componentType)
        {
            entityToSignature_.at(entity).reset(componentType);
        };

        Signature& requestSignature(EntityId entity)
        {
            return entityToSignature_.at(entity);
        };

    private:
        IdManager entityIdManager_;
        std::unordered_map<EntityId, Signature> entityToSignature_;
    };
}

#endif
