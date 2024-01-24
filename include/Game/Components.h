#ifndef COMPONENTS_H_20240110221821
#define COMPONENTS_H_20240110221821

#include "Component.h"
#include "Utility.h"
#include <raylib.h>

// Components
//=================================
struct CDirection
    : public snd::Component<CDirection>
{
public:
    Vector2& getDirection() { return directionVector_; }
    void setDirection(Direction direction)
    {
        switch (direction)
        {
        case RIGHT:
            directionVector_ = V_RIGHT;
            break;

        case LEFT:
            directionVector_ = V_LEFT;
            break;

        default:
        case UP:
            directionVector_ = V_UP;
            break;

        case DOWN:
            directionVector_ = V_DOWN;
            break;
        }
    }

    CDirection(Direction direction = UP)
    {
        setDirection(direction);
    }

private:
    Vector2 directionVector_;
};

struct CPosition
    : public snd::Component<CPosition>
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

struct CRangeMovement
    : public snd::Component<CRangeMovement>
{
public:
    int getMovementRange() { return movementRange_; };
    void setMovementRange(int movementRange) { movementRange_ = movementRange; };

    CRangeMovement(int range)
        : movementRange_(range){};

private:
    int movementRange_;
};

struct CRenderOffset
    : public snd::Component<CRenderOffset>
{
public:
    Vector2& getTransform() { return renderOffset_; };
    static void setTransform(const Vector2& position) { renderOffset_ = position; };

    CRenderOffset(){};

    CRenderOffset(Vector2 position)
    {
        renderOffset_ = position;
    };

    CRenderOffset(float positionX, float positionY)
    {
        renderOffset_ = {positionX, positionY};
    };

private:
    // Offset from root position
    static inline Vector2 renderOffset_{0, 0};
};

struct CTexture
    : public snd::Component<CTexture>
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

// Tags
//=================================

struct TIsCursor : public snd::Component<TIsCursor>
{
};

struct TIsHoverable : public snd::Component<TIsHoverable>
{
};

struct TIsImpassable : public snd::Component<TIsImpassable>
{
};

struct TIsPassable : public snd::Component<TIsPassable>
{
};

struct TIsSelectable : public snd::Component<TIsSelectable>
{
};

struct TIsSelected : public snd::Component<TIsSelected>
{
};

struct TIsReachable : public snd::Component<TIsReachable>
{
};

struct TIsUnderCursor : public snd::Component<TIsUnderCursor>
{
};

struct TKeyControlled : public snd::Component<TKeyControlled>
{
};

struct TMouseControlled : public snd::Component<TMouseControlled>
{
};

struct TRenderedAsMap : public snd::Component<TRenderedAsMap>
{
};

struct TRenderedAsMapOverlay : public snd::Component<TRenderedAsMapOverlay>
{
};

struct TRenderedAsObject : public snd::Component<TRenderedAsObject>
{
};

struct TRenderedAsUI : public snd::Component<TRenderedAsUI>
{
};
//=================================

#endif
