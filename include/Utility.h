#ifndef IG20240317183916
#define IG20240317183916

#include "raylibEx.h"
#include <vector>

namespace Utility
{
    template <typename Type>
    inline void appendVector(std::vector<Type>& targetVector, const std::vector<Type>& vector)
    {
        targetVector.insert(targetVector.end(), vector.begin(), vector.end());
    }

    inline bool isInVector(const Vector2i& element, std::vector<Vector2i> vector)
    {
        for (auto& entry : vector)
        {
            if (Vector2Equals(element, entry))
                return true;
        }

        return false;
    }
}

#endif