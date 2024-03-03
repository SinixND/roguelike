#ifndef _20240203171045
#define _20240203171045

#include <cstddef>
#include <raylib.h>
#include <raymath.h>

#include <functional>

// TYPEDEFS
//=====================================
struct Vector2i
{
    int x;
    int y;
};

struct Matrix2i
{
    int m11, m12;
    int m21, m22;
};

struct Area
{
    int left;   // Rectangle top-left corner position x
    int top;    // Rectangle top-left corner position y
    int width;  // Rectangle width
    int height; // Rectangle height

    int right;
    int bottom;

    Area(int leftX, int topY, int widthX, int heightY)
        : left(leftX)
        , top(topY)
        , width(widthX)
        , height(heightY)
        , right(leftX + widthX - 1)
        , bottom(topY + heightY - 1){};
};
//=====================================

// FUNCTIONS
//=====================================
// in fullscreen mode we can't use GetScreenWidth/Height, so make a function that gets the right data for each mode
inline Vector2 GetDisplaySize()
{
    if (IsWindowFullscreen())
        return Vector2{static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};
    else
        return Vector2{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
}

inline Vector2i MatrixMultiply(const Matrix2i& M, const Vector2i& V)
{
    return Vector2i{((M.m11 * V.x) + (M.m12 * V.y)), ((M.m21 * V.x) + (M.m22 * V.y))};
}

inline size_t Vector2Length(const Vector2i& V)
{
    return abs(V.x) + abs(V.y);
}

inline Rectangle GetRectangle(const Vector2& topLeft, const Vector2& bottomRight)
{
    return Rectangle{
        topLeft.x,
        topLeft.y,
        bottomRight.x - topLeft.x,
        bottomRight.y - topLeft.y};
}

inline Vector2 GetMainDirection(const Vector2& from, const Vector2& to)
{
    Vector2 v{Vector2Subtract(to, from)};
    abs(v.x) > abs(v.y) ? v.y = 0 : v.x = 0;
    return Vector2Normalize(v);
}

inline Rectangle GetRenderRec()
{
    return Rectangle{
        0,
        0,
        static_cast<float>(GetRenderWidth()),
        static_cast<float>(GetRenderHeight())};
}

inline Vector2 ConvertVector2(const Vector2i& v)
{
    return Vector2{static_cast<float>(v.x), static_cast<float>(v.y)};
}
//=====================================

// FUNCTION OVERLOADS
//=====================================

// Add two vectors (v1 + v2)
RMAPI Vector2i Vector2Add(const Vector2i& v1, const Vector2i& v2)
{
    return Vector2i{v1.x + v2.x, v1.y + v2.y};
}

RMAPI Vector2i Vector2AddValue(const Vector2i& v1, int value)
{
    return Vector2i{v1.x + value, v1.y + value};
}

// Subtract two vectors (v1 - v2)
RMAPI Vector2i Vector2Subtract(const Vector2i& v1, const Vector2i& v2)
{
    return Vector2i{v1.x - v2.x, v1.y - v2.y};
}

RMAPI Vector2i Vector2SubtractValue(const Vector2i& v1, int value)
{
    return Vector2i{v1.x - value, v1.y - value};
}

// Scale vector (multiply by value)
RMAPI Vector2 Vector2Scale(const Vector2i& v, float scale)
{
    return Vector2{v.x * scale, v.y * scale};
}

// Check whether two given vectors are almost equal
RMAPI int Vector2Equals(const Vector2i& v1, const Vector2i& v2)
{
    return ((v1.x == v2.x) && (v1.y == v2.y));
}
//=====================================

// OPERATOR OVERLOADS
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
struct std::hash<Vector2i>
{
    size_t operator()(const Vector2i& V) const noexcept
    {
        size_t h1 = std::hash<int>()(V.x);
        size_t h2 = std::hash<int>()(V.y);
        return (h1 ^ (h2 << 1));
    }
};

inline bool operator==(const Vector2i& lhs, const Vector2i& rhs)
{
    return Vector2Equals(lhs, rhs);
};
//=====================================

#endif
