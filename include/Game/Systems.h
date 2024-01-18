#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "Components.h"
#include "EntityId.h"
#include "System.h"

namespace snd
{
    class SMouseControl
        : public System<TMouseControlled, CPosition, CTransformation>
    {
    public:
        void action(EntityId entityId);

        SMouseControl(ECS* ecs)
            : System<TMouseControlled, CPosition, CTransformation>(ecs)
        {
        }
    };

    class SMovement
        : public System<TKeyControlled, CPosition, COrientation, CTransformation>
    {
    public:
        void action(EntityId entityId);

        SMovement(ECS* ecs)
            : System<TKeyControlled, CPosition, COrientation, CTransformation>(ecs)
        {
        }
    };

    class SRenderMap
        : public System<TRenderMap, CTexture, CPosition, COrientation, CTransformation>
    {
    public:
        void action(EntityId entityId);

        SRenderMap(ECS* ecs)
            : System<TRenderMap, CTexture, CPosition, COrientation, CTransformation>(ecs){};
    };

    class SRenderObjects
        : public System<TRenderObject, CTexture, CPosition, COrientation, CTransformation>
    {
    public:
        void action(EntityId entityId);

        SRenderObjects(ECS* ecs)
            : System<TRenderObject, CTexture, CPosition, COrientation, CTransformation>(ecs){};
    };

    class SRenderUI
        : public System<TRenderUI, CTexture, CPosition, COrientation, CTransformation>
    {
    public:
        void action(EntityId entityId);

        SRenderUI(ECS* ecs)
            : System<TRenderUI, CTexture, CPosition, COrientation, CTransformation>(ecs){};
    };

    class SRotation
        : public System<TKeyControlled, COrientation>
    {
    public:
        void action(EntityId entityId);

        SRotation(ECS* ecs)
            : System<TKeyControlled, COrientation>(ecs)
        {
        }
    };

    // System template
    /*
        class CustomSystem
            : public System<ComponentTypes>
        {
        public:
            void action(EntityId entityId)
            {
                // Get components
                // auto& component{ecs_->retrieveComponent<ComponentType>(entityId)->component_};

                // Action

            }
            CustomSystem(ECS* ecs)
                : System<ComponentTypes>(ecs)
            {
            }
        }
    */
}

#endif
