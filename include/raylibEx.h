#ifndef IG20240203171045
#define IG20240203171045

#include <cmath>
#include <cstddef>
#include <functional>
#include <raylib.h>
#include <raymath.h>

// Typedefs
//=====================================
class RectangleEx
{
public:
    float left() const { return left_; }

    RectangleEx& resizeLeft(float left)
    {
        left_ = left;
        width_ = right_ - left_;
        return *this;
    }

    RectangleEx& moveLeft(float left)
    {
        left_ = left;
        right_ = left_ + width_;
        return *this;
    }

    float right() const { return right_; }

    RectangleEx& resizeRight(float right)
    {
        right_ = right;
        width_ = right_ - left_;
        return *this;
    }

    RectangleEx& moveRight(float right)
    {
        right_ = right;
        left_ = right_ - width_;
        return *this;
    }

    float top() const { return top_; }

    RectangleEx& resizeTop(float top)
    {
        top_ = top;
        height_ = bottom_ - top_;
        return *this;
    }

    RectangleEx& moveTop(float top)
    {
        top_ = top;
        bottom_ = top_ + height_;
        return *this;
    }

    float bottom() const { return bottom_; }

    RectangleEx& resizeBottom(float bottom)
    {
        bottom_ = bottom;
        height_ = bottom_ - top_;
        return *this;
    }

    RectangleEx& moveBottom(float bottom)
    {
        bottom_ = bottom;
        top_ = bottom_ - height_;
        return *this;
    }

    float width() const { return width_; }

    RectangleEx& resizeWidthLeft(float width)
    {
        width_ = width;
        left_ = right_ - width_;
        return *this;
    }

    RectangleEx& resizeWidthRight(float width)
    {
        width_ = width;
        right_ = left_ + width_;
        return *this;
    }

    float height() const { return height_; }

    RectangleEx& resizeHeightTop(float height)
    {
        height_ = height;
        top_ = bottom_ - height_;
        return *this;
    }

    RectangleEx& resizeHeightBottom(float height)
    {
        height_ = height;
        bottom_ = top_ + height_;
        return *this;
    }

    Vector2 topLeft() { return Vector2{left_, top_}; }

    Vector2 bottomRight() { return Vector2{right_, bottom_}; }

    Rectangle rectangle()
    {
        return Rectangle{
          left_,
          top_,
          width_,
          height_};
    }

    RectangleEx(float left, float top, float width = 1, float height = 1)
        : left_(left)
        , top_(top)
        , right_(left + width - 1)
        , bottom_(top + height - 1)
        , width_(width)
        , height_(height)
    {
    }

    RectangleEx(Vector2 topLeft, Vector2 bottomRight = {0, 0})
        : left_(topLeft.x)
        , top_(topLeft.y)
        , right_(bottomRight.x)
        , bottom_(bottomRight.y)
        , width_(bottomRight.x - topLeft.x + 1)
        , height_(bottomRight.y - topLeft.y + 1)
    {
    }

    RectangleEx(Rectangle rectangle)
        : left_(rectangle.x)
        , top_(rectangle.y)
        , right_(rectangle.x + rectangle.width - 1)
        , bottom_(rectangle.y + rectangle.height - 1)
        , width_(rectangle.width)
        , height_(rectangle.height)
    {
    }

private:
    float left_;
    float top_;
    float right_;
    float bottom_;
    float width_;
    float height_;
};

struct Vector2I
{
    int x;
    int y;

    Vector2I& operator+=(Vector2I const& right)
    {
        this->x += right.x;
        this->y += right.y;
        return *this;
    }

    Vector2I& operator-=(Vector2I const& right)
    {
        this->x -= right.x;
        this->y -= right.y;
        return *this;
    }
};

struct Matrix2x2
{
    float m11, m12;
    float m21, m22;
};

struct Matrix2x2I
{
    int m11, m12;
    int m21, m22;
};

struct RectangleExI
{
    int left;
    int top;
    int right;
    int bottom;

    int width;
    int height;

    RectangleExI(int left = 0, int top = 0, int width = 1, int height = 1)
        : left(left)
        , top(top)
        , right(left + width - 1)
        , bottom(top + height - 1)
        , width(width)
        , height(height)
    {
    }

    RectangleExI(Vector2I center, int width, int height)
        : left(center.x - (width / 2))
        , top(center.y - (height / 2))
        , right(center.x + (width / 2))
        , bottom(center.y + (height / 2))
        , width(width)
        , height(height)
    {
    }

    RectangleExI(Vector2I from, Vector2I to)
        : left(from.x)
        , top(from.y)
        , right(to.x)
        , bottom(to.y)
        , width(to.x - from.x + 1)
        , height(to.y - from.y + 1)
    {
        // Verify left < right, top < bottom and width/height > 0
        if (left > right)
        {
            int temp = left;
            left = right;
            right = temp;
            width = right - left + 1;
        }

        if (top > bottom)
        {
            int temp = top;
            top = bottom;
            bottom = temp;
            height = bottom - top + 1;
        }
    }
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

inline int Vector2Sum(Vector2I V)
{
    return abs(V.x) + abs(V.y);
}

inline Rectangle GetRectangle(Vector2 topLeft, Vector2 bottomRight)
{
    return Rectangle{
      topLeft.x,
      topLeft.y,
      bottomRight.x - topLeft.x,
      bottomRight.y - topLeft.y};
}

inline Vector2 GetMainDirection(Vector2 from, Vector2 to)
{
    Vector2 v{Vector2Subtract(to, from)};

    (abs(v.x) > abs(v.y))
      ? (v.y = 0)
      : (v.x = 0);

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

inline Vector2 ConvertToVector2(Vector2I V)
{
    return Vector2{
      static_cast<float>(V.x),
      static_cast<float>(V.y)};
}

inline Vector2I ConvertToVector2I(Vector2 V)
{
    return Vector2I{
      static_cast<int>(V.x),
      static_cast<int>(V.y)};
}

inline Vector2I GetMin(Vector2I V1, Vector2I V2)
{
    return Vector2I{
      (V1.x < V2.x ? V1.x : V2.x),
      (V1.y < V2.y ? V1.y : V2.y)};
}

inline Vector2I GetMax(Vector2I V1, Vector2I V2)
{
    return Vector2I{
      (V1.x > V2.x ? V1.x : V2.x),
      (V1.y > V2.y ? V1.y : V2.y)};
}

inline bool CheckCollisionPointRectangleEx(Vector2I tilePosition, RectangleExI const& rectangle)
{
    bool isColliding = false;

    if ((tilePosition.x >= rectangle.left) && (tilePosition.x < rectangle.right) && (tilePosition.y >= rectangle.top) && (tilePosition.y < rectangle.bottom)) isColliding = true;

    return isColliding;
}
//=====================================

// Function Overloads
//=====================================
// Add two vectors (v1 + v2)
RMAPI Vector2I Vector2Add(Vector2I v1, Vector2I v2)
{
    return Vector2I{
      v1.x + v2.x,
      v1.y + v2.y};
}

RMAPI Vector2I Vector2AddValue(Vector2I v1, int value)
{
    return Vector2I{
      v1.x + value,
      v1.y + value};
}

// Subtract two vectors (v1 - v2)
RMAPI Vector2I Vector2Subtract(Vector2I v1, Vector2I v2)
{
    return Vector2I{
      v1.x - v2.x,
      v1.y - v2.y};
}

RMAPI Vector2I Vector2SubtractValue(Vector2I v1, int value)
{
    return Vector2I{
      v1.x - value,
      v1.y - value};
}

// Scale vector (multiply by value)
RMAPI Vector2 Vector2Scale(Vector2I v, float scale)
{
    return Vector2{
      v.x * scale,
      v.y * scale};
}

RMAPI Vector2I Vector2Scale(Vector2I v, int scale)
{
    return Vector2I{
      v.x * scale,
      v.y * scale};
}

// Check whether two given vectors are almost equal
RMAPI int Vector2Equals(Vector2I v1, Vector2I v2)
{
    return ((v1.x == v2.x) && (v1.y == v2.y));
}

RMAPI Vector2I Vector2Transform(Matrix2x2I M, Vector2I V)
{
    return Vector2I{
      ((M.m11 * V.x) + (M.m12 * V.y)),
      ((M.m21 * V.x) + (M.m22 * V.y))};
}

RMAPI Vector2 Vector2Transform(Matrix2x2 M, Vector2 V)
{
    return Vector2{
      ((M.m11 * V.x) + (M.m12 * V.y)),
      ((M.m21 * V.x) + (M.m22 * V.y))};
}

inline bool CheckCollisionPointRec(Vector2 point, RectangleEx rec)
{
    return CheckCollisionPointRec(point, rec.rectangle());
}
//=====================================

// Operator Overloads
//=====================================
inline Vector2& operator+=(Vector2& left, Vector2 const& right)
{
    left.x += right.x;
    left.y += right.y;
    return left;
}

inline Vector2& operator-=(Vector2& left, Vector2 const& right)
{
    left.x -= right.x;
    left.y -= right.y;
    return left;
}

inline bool operator==(Vector2 lhs, Vector2 rhs)
{
    return Vector2Equals(lhs, rhs);
}

inline bool operator==(Vector2I lhs, Vector2I rhs)
{
    return Vector2Equals(lhs, rhs);
}

template <>
struct std::hash<Vector2>
{
    size_t operator()(Vector2 V) const noexcept
    {
        size_t h1 = std::hash<double>()(V.x);
        size_t h2 = std::hash<double>()(V.y);
        return (h1 ^ (h2 << 1));
    }
};

template <>
struct std::hash<Vector2I>
{
    size_t operator()(Vector2I V) const noexcept
    {
        size_t h1 = std::hash<int>()(V.x);
        size_t h2 = std::hash<int>()(V.y);
        return (h1 ^ (h2 << 1));
    }
};
//=====================================

#endif
