#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "Components.h"
#include "EntityId.h"
#include "System.h"

namespace snd
{
    class SRender
        : public System<CTexture, CPosition, COrientation, CTransformation>
    {
    public:
        void action(EntityId entityId);

        SRender(ECS* ecs)
            : System<CTexture, CPosition, COrientation, CTransformation>(ecs){};
    };

    class SMouseControl
        : public System<FMouseControlled, CPosition, CTransformation>
    {
    public:
        void action(EntityId entityId);

        SMouseControl(ECS* ecs)
            : System<FMouseControlled, CPosition, CTransformation>(ecs)
        {
        }
    };

    class SRotation
        : public System<FKeyControlled, COrientation>
    {
    public:
        void action(EntityId entityId);

        SRotation(ECS* ecs)
            : System<FKeyControlled, COrientation>(ecs)
        {
        }
    };

    class SMovement
        : public System<FKeyControlled, CPosition, COrientation, CTransformation>
    {
    public:
        void action(EntityId entityId);

        SMovement(ECS* ecs)
            : System<FKeyControlled, CPosition, COrientation, CTransformation>(ecs)
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
