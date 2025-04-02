#ifndef IG20240317183916
#define IG20240317183916

#include "Colors.h"
#include "raylibEx.h"

namespace Utility
{
    namespace RotationMatrices
    {
        Matrix2x2I constexpr NONE{
            1,
            0,
            0,
            1
        };

        Matrix2x2I constexpr CW90{
            0,
            -1,
            1,
            0
        };

        Matrix2x2I constexpr CCW90{
            0,
            1,
            -1,
            0
        };

        Matrix2x2 constexpr CW45{
            0.7071068,
            -0.7071068,
            0.7071068,
            0.7071068
        };
    }
}
#endif
