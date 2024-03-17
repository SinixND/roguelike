#ifndef IG20240317004321
#define IG20240317004321

#include <vector>

template <typename Type>
inline void appendVector(std::vector<Type>& targetVector, const std::vector<Type>& vector)
{
    targetVector.insert(targetVector.end(), vector.begin(), vector.end());
}

#endif