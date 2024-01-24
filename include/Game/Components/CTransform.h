#ifndef CTRANSFORM_H_20240128153451
#define CTRANSFORM_H_20240128153451

#include "Component.h"
#include <raylib.h>

struct CTransform
    : public snd::Component<CTransform>
{
public:
    Vector2& getTransform() { return transform_; };
    static void setTransform(const Vector2& position) { transform_ = position; };

    CTransform(){};

    CTransform(Vector2 position)
    {
        transform_ = position;
    };

    CTransform(float positionX, float positionY)
    {
        transform_ = {positionX, positionY};
    };

private:
    // Offset from root position
    static inline Vector2 transform_{0, 0};
};

#endif