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
    float const& left() const { return left_; }

    void resizeLeft(float left)
    {
        left_ = left;
        width_ = right_ - left_;
    }

    void moveLeft(float left)
    {
        left_ = left;
        right_ = left_ + width_;
    }

    float const& right() const { return right_; }

    void resizeRight(float right)
    {
        right_ = right;
        width_ = right_ - left_;
    }

    void moveRight(float right)
    {
        right_ = right;
        left_ = right_ - width_;
    }

    float const& top() const { return top_; }

    void resizeTop(float top)
    {
        top_ = top;
        height_ = bottom_ - top_;
    }

    void moveTop(float top)
    {
        top_ = top;
        bottom_ = top_ + height_;
    }

    float const& bottom() const { return bottom_; }

    void resizeBottom(float bottom)
    {
        bottom_ = bottom;
        height_ = bottom_ - top_;
    }

    void moveBottom(float bottom)
    {
        bottom_ = bottom;
        top_ = bottom_ - height_;
    }

    float const& width() const { return width_; }

    void resizeWidthLeft(float width)
    {
        width_ = width;
        left_ = right_ - width_;
    }

    void resizeWidthRight(float width)
    {
        width_ = width;
        right_ = left_ + width_;
    }

    float const& height() const { return height_; }

    void resizeHeightTop(float height)
    {
        height_ = height;
        top_ = bottom_ - height_;
    }

    void resizeHeightBottom(float height)
    {
        height_ = height;
        bottom_ = top_ + height_;
    }

    Vector2 topLeft() { return Vector2{left_, top_}; }

    Vector2 bottomRight() { return Vector2{right_, bottom_}; }

    Rectangle rectangle() { return Rectangle{left_, top_, width_, height_}; }

    RectangleEx(float left, float top, float width, float height)
        : left_(left)
        , top_(top)
        , right_(left + width)
        , bottom_(top + height)
        , width_(width)
        , height_(height)
    {}

    RectangleEx(Vector2 topLeft, Vector2 bottomRight)
        : left_(topLeft.x)
        , top_(topLeft.y)
        , right_(bottomRight.x)
        , bottom_(bottomRight.y)
        , width_(bottomRight.x - topLeft.x)
        , height_(bottomRight.y - topLeft.y)
    {}

    RectangleEx(Rectangle rectangle)
        : left_(rectangle.x)
        , top_(rectangle.y)
        , right_(rectangle.x + rectangle.width)
        , bottom_(rectangle.y + rectangle.height)
        , width_(rectangle.width)
        , height_(rectangle.height)
    {}

private:
    float left_;
    float top_;
    float right_;
    float bottom_;
    float width_;
    float height_;
};

struct Vector2i {
    int x;
    int y;
};

struct Matrix2x2 {
    float m11, m12;
    float m21, m22;
};

struct Matrix2x2i {
    int m11, m12;
    int m21, m22;
};

struct Area {
    int left;      // Rectangle top-left corner position x
    int top;       // Rectangle top-left corner position y
    size_t width;  // Rectangle width
    size_t height; // Rectangle height
    int right;
    int bottom;

    Area(int left = 0, int top = 0, int right = 0, int bottom = 0)
        : left(left)
        , top(top)
        , width(right - left + 1)
        , height(bottom - top + 1)
        , right(right)
        , bottom(bottom)
    {}

    Area(int left, int top, size_t width, size_t height)
        : left(left)
        , top(top)
        , width(width)
        , height(height)
        , right(left + width - 1)
        , bottom(top + height - 1)
    {}

    Area(Vector2i center, size_t width, size_t height)
        : left(center.x - (width / 2))
        , top(center.y - (height / 2))
        , width(width)
        , height(height)
        , right(center.x + (width / 2))
        , bottom(center.y + (height / 2))
    {
        [[maybe_unused]] int i{0};
    }

    Area(Vector2i from, Vector2i to)
        : left(from.x)
        , top(from.y)
        , width(to.x - from.x + 1)
        , height(to.y - from.y + 1)
        , right(to.x)
        , bottom(to.y)
    {

        // Verify left < right, top < bottom and width/height > 0
        if (left > right) {
            int temp = left;
            left = right;
            right = temp;
            width = right - left + 1;
        }

        if (top > bottom) {
            int temp = top;
            top = bottom;
            bottom = temp;
            height = bottom - top + 1;
        }
    }
};

struct Line {
    Vector2 start;
    Vector2 end;
};

//=====================================
// Functions
//=====================================
// in fullscreen mode we can't use GetScreenWidth/Height, so make a function
// that gets the right data for each mode
inline Vector2 GetDisplaySize()
{
    if (IsWindowFullscreen())
        return Vector2{
          static_cast<float>(GetMonitorWidth(GetCurrentMonitor())),
          static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};

    else
        return Vector2{
          static_cast<float>(GetRenderWidth()),
          static_cast<float>(GetRenderHeight())};
}

inline size_t Vector2Sum(Vector2i V) { return abs(V.x) + abs(V.y); }

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

inline Vector2 ConvertToVector2(Vector2i V)
{
    return Vector2{static_cast<float>(V.x), static_cast<float>(V.y)};
}

inline Vector2i ConvertToVector2i(Vector2 V)
{
    return Vector2i{static_cast<int>(V.x), static_cast<int>(V.y)};
}

inline Vector2i GetMin(Vector2i V1, Vector2i V2)
{
    return Vector2i{(V1.x < V2.x ? V1.x : V2.x), (V1.y < V2.y ? V1.y : V2.y)};
}

inline Vector2i GetMax(Vector2i V1, Vector2i V2)
{
    return Vector2i{(V1.x > V2.x ? V1.x : V2.x), (V1.y > V2.y ? V1.y : V2.y)};
}

inline bool CheckCollisionPointArea(Vector2i tilePosition, Area const& area)
{
    bool isColliding = false;

    if (
      (tilePosition.x >= area.left) && (tilePosition.x < area.right) &&
      (tilePosition.y >= area.top) && (tilePosition.y < area.bottom))
        isColliding = true;
    return isColliding;
}

//=====================================
// Function Overloads
//=====================================
// Add two vectors (v1 + v2)
RMAPI Vector2i Vector2Add(Vector2i v1, Vector2i v2)
{
    return Vector2i{v1.x + v2.x, v1.y + v2.y};
}

RMAPI void addToVector2(Vector2& v1, Vector2 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
}

RMAPI void addToVector2(Vector2i& v1, Vector2i v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
}

RMAPI Vector2i Vector2AddValue(Vector2i v1, int value)
{
    return Vector2i{v1.x + value, v1.y + value};
}

// Subtract two vectors (v1 - v2)
RMAPI Vector2i Vector2Subtract(Vector2i v1, Vector2i v2)
{
    return Vector2i{v1.x - v2.x, v1.y - v2.y};
}

RMAPI Vector2i Vector2SubtractValue(Vector2i v1, int value)
{
    return Vector2i{v1.x - value, v1.y - value};
}

// Scale vector (multiply by value)
RMAPI Vector2 Vector2Scale(Vector2i v, float scale)
{
    return Vector2{v.x * scale, v.y * scale};
}

RMAPI Vector2i Vector2Scale(Vector2i v, int scale)
{
    return Vector2i{v.x * scale, v.y * scale};
}

// Check whether two given vectors are almost equal
RMAPI int Vector2Equals(Vector2i v1, Vector2i v2)
{
    return ((v1.x == v2.x) && (v1.y == v2.y));
}

RMAPI Vector2i Vector2Transform(Matrix2x2i M, Vector2i V)
{
    return Vector2i{
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
template <>
struct std::hash<Vector2> {
    size_t operator()(Vector2 V) const noexcept
    {
        size_t h1 = std::hash<double>()(V.x);
        size_t h2 = std::hash<double>()(V.y);
        return (h1 ^ (h2 << 1));
    }
};

inline bool operator==(Vector2 lhs, Vector2 rhs)
{
    return Vector2Equals(lhs, rhs);
}

template <>
struct std::hash<Vector2i> {
    size_t operator()(Vector2i V) const noexcept
    {
        size_t h1 = std::hash<int>()(V.x);
        size_t h2 = std::hash<int>()(V.y);
        return (h1 ^ (h2 << 1));
    }
};

inline bool operator==(Vector2i lhs, Vector2i rhs)
{
    return Vector2Equals(lhs, rhs);
}

//=====================================
#endif
