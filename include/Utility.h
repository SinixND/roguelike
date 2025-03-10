#ifndef IG20240317183916
#define IG20240317183916

#include "raylibEx.h"

namespace RotationMatrices
{
    Matrix2x2I constexpr ROTATION_NONE{
        1,
        0,
        0,
        1
    };

    Matrix2x2I constexpr rotationCW{
        0,
        -1,
        1,
        0
    };

    Matrix2x2I constexpr rotationCCW{
        0,
        1,
        -1,
        0
    };

    Matrix2x2 constexpr rotationCW45{
        0.7071068,
        -0.7071068,
        0.7071068,
        0.7071068
    };
}

#endif
