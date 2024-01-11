#ifndef COMPONENTS_H_20240110221821
#define COMPONENTS_H_20240110221821

#include "Component.h"
#include "raylib.h"
#include <functional>
#include <memory_resource>

namespace snd
{
    // Components
    //=================================
    typedef enum
    {
        LEFT,
        RIGHT,
        UP,
        DOWN,
    } Direction;

    struct DirectionComponent
        : public Component<DirectionComponent>
    {
        Vector2 directionVector_;
        float directionDeg_;

        DirectionComponent(Direction direction = RIGHT)
        {
            switch (direction)
            {
            default:
            case RIGHT:
                directionVector_ = {1, 0};
                directionDeg_ = 0;
                break;

            case LEFT:
                directionVector_ = {-1, 0};
                directionDeg_ = 180;
                break;

            case UP:
                directionVector_ = {0, -1};
                directionDeg_ = -90;
                break;

            case DOWN:
                directionVector_ = {0, 1};
                directionDeg_ = 90;
                break;
            }
        }
    };

    struct PositionComponent
        : public Component<PositionComponent>
    {
        Vector2 position_;

        PositionComponent(float x, float y)
            : position_(x, y){};

        PositionComponent(Vector2 position = {0, 0})
            : position_(position){};
    };

    struct TextureComponent
        : public Component<TextureComponent>
    {
        Texture2D* texture_;

        TextureComponent(Texture2D* texture)
            : texture_(texture){};
    };

    struct TransformComponent
        : public Component<TransformComponent>
    {
        Vector2 transform_;

        TransformComponent(float x, float y)
            : transform_(x, y){};

        TransformComponent(Vector2 transform = {0, 0})
            : transform_(transform){};
    };
    //=================================

    // Flags
    //=================================
    struct MouseControlFlag
        : public Component<MouseControlFlag>
    {
    };

    struct ControlFlag
        : public Component<ControlFlag>
    {
    };
    //=================================
}

#endif