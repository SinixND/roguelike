#ifndef RAYLIBEXTENTION_H_20240122021744
#define RAYLIBEXTENTION_H_20240122021744

#include <raylib.h>

typedef struct
{
    int m11, m12;
    int m21, m22;
} Matrix2x2;

inline Vector2 intifyVector2(Vector2 V)
{
    return Vector2(static_cast<int>(V.x) * 1.0f, static_cast<int>(V.y) * 1.0f);
}

inline Vector2 Vector2MatrixMultiply(const Vector2& V, const Matrix2x2& M)
{
    return Vector2{(M.m11 * V.x + M.m12 * V.y), (M.m21 * V.x + M.m22 * V.y)};
}

#endif
