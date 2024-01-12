#ifndef COMPONENTS_H_20240110221821
#define COMPONENTS_H_20240110221821

#include "Component.h"
#include "Utility.h"
#include "raylib.h"
#include "raymath.h"
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
    } Rotation;

    struct RotationComponent
        : public Component<RotationComponent>
    {
        Vector2 directionVector_;
        float rotationDeg_;

        RotationComponent(Rotation rotation = UP)
        {
            switch (rotation)
            {
            case RIGHT:
                directionVector_ = {1, 0};
                rotationDeg_ = 90;
                break;

            case LEFT:
                directionVector_ = {-1, 0};
                rotationDeg_ = -90;
                break;

            default:
            case UP:
                directionVector_ = {0, -1};
                rotationDeg_ = 0;
                break;

            case DOWN:
                directionVector_ = {0, 1};
                rotationDeg_ = 180;
                break;
            }
        }
    };

    struct PositionComponent
        : public Component<PositionComponent>
    {
        // Tile position
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
        // Offset from game tile position to screen tile position
        static inline Vector2 transform_;

        TransformComponent(Vector2 position = {0, 0})
        {
            transform_ = position;
        };

        TransformComponent(float positionX, float positionY)
        {
            transform_ = {positionX, positionY};
        };
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
