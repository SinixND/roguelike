#ifndef IG20240317183916
#define IG20240317183916

#include "raylibEx.h"
#include <vector>

namespace snx
{
    template <typename Type>
    inline void appendVector(std::vector<Type>& targetVector, std::vector<Type> const& vector)
    {
        targetVector.insert(targetVector.end(), vector.begin(), vector.end());
    }

    inline bool isInVector(Vector2I element, std::vector<Vector2I> vector)
    {
        for (auto& entry : vector)
        {
            if (Vector2Equals(element, entry))
            {
                return true;
            }
        }

        return false;
    }
}

#endif
