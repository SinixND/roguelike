#ifndef CTRANSFORM_H_20240128153451
#define CTRANSFORM_H_20240128153451

#include "Component.h"
#include <raylibEx.h>

struct CTransform
    : public snd::Component<CTransform>
{
public:
    Vector2Int& getTransform() { return transform_; };
    static void setTransform(const Vector2Int& position) { transform_ = position; };

    CTransform(){};

    CTransform(Vector2Int position)
    {
        transform_ = position;
    };

    CTransform(int positionX, int positionY)
    {
        transform_ = {positionX, positionY};
    };

private:
    // Offset from root position
    static inline Vector2Int transform_{0, 0};
};

#endif