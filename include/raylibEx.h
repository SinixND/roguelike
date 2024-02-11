#ifndef _20240203171045
#define _20240203171045

#include <cstddef>
#include <raylib.h>
#include <raymath.h>

#include <functional>

// Typedefs
//=====================================
struct Vector2Int
{
    int x{};
    int y{};
};

struct Matrix2x2Int
{
    int m11{}, m12{};
    int m21{}, m22{};
};

struct Box
{
    int left{};   // Rectangle top-left corner position x
    int top{};    // Rectangle top-left corner position y
    int width{};  // Rectangle width
    int height{}; // Rectangle height

    int right{};
    int bottom{};

    Box(int leftX, int topY, int widthX, int heightY)
        : left(leftX)
        , top(topY)
        , width(widthX)
        , height(heightY)
        , right(leftX + widthX - 1)
        , bottom(topY + heightY - 1){};
};
//=====================================

// Functions
//=====================================
// in fullscreen mode we can't use GetScreenWidth/Height, so make a function that gets the right data for each mode
inline Vector2 GetDisplaySize()
{
    if (IsWindowFullscreen())
        return Vector2{static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};
    else
        return Vector2{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
}

inline Vector2Int Vector2MatrixMultiply(const Matrix2x2Int& M, const Vector2Int& V)
{
    return Vector2Int{((M.m11 * V.x) + (M.m12 * V.y)), ((M.m21 * V.x) + (M.m22 * V.y))};
}

inline size_t Vector2LengthTiled(const Vector2Int& V)
{
    return abs(V.x) + abs(V.y);
}
//=====================================

// Function overloads
//=====================================

// Add two vectors (v1 + v2)
RMAPI Vector2Int Vector2Add(Vector2Int v1, Vector2Int v2)
{
    Vector2Int result = {v1.x + v2.x, v1.y + v2.y};

    return result;
}

// Subtract two vectors (v1 - v2)
RMAPI Vector2Int Vector2Subtract(Vector2Int v1, Vector2Int v2)
{
    Vector2Int result = {v1.x - v2.x, v1.y - v2.y};

    return result;
}

// Scale vector (multiply by value)
RMAPI Vector2 Vector2Scale(Vector2Int v, float scale)
{
    Vector2 result = {v.x * scale, v.y * scale};

    return result;
}

// Check whether two given vectors are almost equal
RMAPI int Vector2Equals(Vector2Int p, Vector2Int q)
{
    return ((p.x == q.x) && (p.y == q.y));
}

//=====================================

// Operator overloads
//=====================================
template <>
struct std::hash<Vector2>
{
    size_t operator()(const Vector2& V) const noexcept
    {
        size_t h1 = std::hash<double>()(V.x);
        size_t h2 = std::hash<double>()(V.y);
        return (h1 ^ (h2 << 1));
    }
};

inline bool operator==(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2Equals(lhs, rhs);
};

template <>
struct std::hash<Vector2Int>
{
    size_t operator()(const Vector2Int& V) const noexcept
    {
        size_t h1 = std::hash<int>()(V.x);
        size_t h2 = std::hash<int>()(V.y);
        return (h1 ^ (h2 << 1));
    }
};

inline bool operator==(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return Vector2Equals(lhs, rhs);
};
//=====================================

#endif
