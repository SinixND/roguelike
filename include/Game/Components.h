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
    struct CPosition
        : public Component<CPosition>
    {
    public:
        Vector2& getPosition() { return position_; };
        void setPosition(const Vector2& position) { position_ = position; };

        CPosition(float x, float y)
            : position_(x, y){};

        CPosition(int x, int y)
            : position_(static_cast<float>(x), static_cast<float>(y)){};

        CPosition(Vector2 position = {0, 0})
            : position_(position){};

    private:
        // Tile position
        Vector2 position_;
    };

    typedef enum
    {
        LEFT,
        RIGHT,
        UP,
        DOWN,
    } Direction;

    struct CRotation
        : public Component<CRotation>
    {
    public:
        Vector2& getDirection() { return directionVector_; }
        float& getRotation() { return rotationDeg_; }
        void setDirection(Direction direction)
        {
            switch (direction)
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

        CRotation(Direction direction = UP)
        {
            setDirection(direction);
        }

    private:
        Vector2 directionVector_;
        float rotationDeg_;
    };

    struct CTransformation
        : public Component<CTransformation>
    {
    public:
        Vector2& getTransform() { return transform_; };
        void setTransform(const Vector2& position) { transform_ = position; };

        CTransformation(Vector2 position = {0, 0})
        {
            transform_ = position;
        };

        CTransformation(float positionX, float positionY)
        {
            transform_ = {positionX, positionY};
        };

    private:
        // Offset from root position
        Vector2 transform_;
    };

    struct CTexture
        : public Component<CTexture>
    {
    public:
        Texture2D* getTexture() { return texture_; };
        void setTexture(Texture2D* texture) { texture_ = texture; };

        CTexture(Texture2D* texture)
            : texture_(texture){};

    private:
        Texture2D* texture_;
    };
    //=================================

    // Flags
    //=================================
    struct FMouseControlled
        : public Component<FMouseControlled>
    {
    };

    struct FKeyControlled
        : public Component<FKeyControlled>
    {
    };

    struct FCollision
        : public Component<FCollision>
    {
    };
    //=================================
}

#endif
