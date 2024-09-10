#ifndef IG20240414161243
#define IG20240414161243

#include "raylibEx.h"

namespace RotationMatrices
{
    Matrix2x2I constexpr M_ROTATE_NONE{
        1,
        0,
        0,
        1};

    Matrix2x2I constexpr M_ROTATE_CW{
        0,
        -1,
        1,
        0};

    Matrix2x2I constexpr M_ROTATE_CCW{
        0,
        1,
        -1,
        0};

    Matrix2x2 constexpr M_ROTATE_CW_45{
        0.7071068,
        -0.7071068,
        0.7071068,
        0.7071068};
}

#endif
