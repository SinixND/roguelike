#ifndef IG20250506234330
#define IG20250506234330

#include <vector>

struct Vector2I;

namespace PathSystem
{
    std::vector<Vector2I> const& update(
        std::vector<Vector2I>& heroPath
    );
}

#endif
