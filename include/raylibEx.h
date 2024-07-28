#ifndef IG20240203171045
#define IG20240203171045

#include <cassert>
#include <cmath>
#include <cstddef>
#include <format>
#include <functional>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>

// Typedefs
//=====================================
class RectangleEx
{
    float left_;
    float top_;
    float right_;
    float bottom_;
    float width_;
    float height_;

public:
    RectangleEx() = default;

    RectangleEx(float left, float top, float width = 1, float height = 1)
        : left_(left)
        , top_(top)
        , right_(left + width - 1)
        , bottom_(top + height - 1)
        , width_(width)
        , height_(height)
    {
        setLeft(left);
        setTop(top);
        resizeWidthRight(width);
        resizeHeightBottom(height);
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

    float left() const { return left_; }

    RectangleEx& setLeft(float left)
    {
        left_ = left;
        updateWidth();
        return *this;
    }

    RectangleEx& offsetLeft(float left)
    {
        left_ += left;
        updateWidth();
        return *this;
    }

    float right() const { return right_; }

    RectangleEx& setRight(float right)
    {
        right_ = right;
        updateWidth();
        return *this;
    }

    RectangleEx& offsetRight(float right)
    {
        right_ += right;
        updateWidth();
        return *this;
    }

    float top() const { return top_; }

    RectangleEx& setTop(float top)
    {
        top_ = top;
        updateHeight();
        return *this;
    }

    RectangleEx& offsetTop(float top)
    {
        top_ += top;
        updateHeight();
        return *this;
    }

    float bottom() const { return bottom_; }

    RectangleEx& setBottom(float bottom)
    {
        bottom_ = bottom;
        updateHeight();
        return *this;
    }

    RectangleEx& offsetBottom(float bottom)
    {
        bottom_ += bottom;
        updateHeight();
        return *this;
    }

    float width() const { return width_; }

    RectangleEx& resizeWidthRight(float width)
    {
        width_ = width;
        updateRight();
        return *this;
    }

    RectangleEx& resizeWidthLeft(float width)
    {
        width_ = width;
        updateLeft();
        return *this;
    }

    float height() const { return height_; }

    RectangleEx& resizeHeightBottom(float height)
    {
        height_ = height;
        updateBottom();
        return *this;
    }

    RectangleEx& resizeHeightTop(float height)
    {
        height_ = height;
        updateTop();
        return *this;
    }

    Vector2 topLeft() const
    {
        return Vector2{
            left_,
            top_};
    }

    Vector2 bottomRight() const
    {
        return Vector2{
            right_,
            bottom_};
    }

    Vector2 center() const
    {
        return Vector2{
            (left_ + (width_ / 2)),
            (top_ + (height_ / 2))};
    }

    Rectangle rectangle() const
    {
        return Rectangle{
            left_,
            top_,
            width_,
            height_};
    }

private:
    void updateWidth()
    {
        width_ = right_ - left_ + 1;
    }

    void updateHeight()
    {
        height_ = bottom_ - top_ + 1;
    }

    void updateLeft()
    {
        left_ = right_ - width_ + 1;
    }

    void updateTop()
    {
        top_ = bottom_ - height_ + 1;
    }

    void updateRight()
    {
        right_ = left_ + width_ - 1;
    }

    void updateBottom()
    {
        bottom_ = top_ + height_ - 1;
    }
};

struct Vector2I
{
    int x;
    int y;
};

class RectangleExI
{
    int left_;
    int top_;
    int right_;
    int bottom_;
    int width_;
    int height_;

public:
    RectangleExI() = default;

    RectangleExI(int left, int top, int width = 1, int height = 1)
        : left_(left)
        , top_(top)
        , right_(left + width - 1)
        , bottom_(top + height - 1)
        , width_(width)
        , height_(height)
    {
        setLeft(left);
        setTop(top);
        resizeWidthRight(width);
        resizeHeightBottom(height);
    }

    RectangleExI(Vector2I topLeft, Vector2I bottomRight = {0, 0})
        : left_(topLeft.x)
        , top_(topLeft.y)
        , right_(bottomRight.x)
        , bottom_(bottomRight.y)
        , width_(bottomRight.x - topLeft.x + 1)
        , height_(bottomRight.y - topLeft.y + 1)
    {
    }

    // Only odd values for width/height
    RectangleExI(Vector2I center, int width, int height)
        : left_(center.x - (width / 2))
        , top_(center.y - (height / 2))
        , right_(center.x + (width / 2))
        , bottom_(center.y + (height / 2))
        , width_(width)
        , height_(height)
    {
        if (
            !(width % 2)
            || !(height % 2))
        {
            std::cerr << "[ERROR] Width (" << width << ") or height (" << height << ")invalid; Must be odd value!\n";
            assert(0);
        }
    }

    int left() const { return left_; }

    RectangleExI& setLeft(int left)
    {
        left_ = left;
        updateWidth();
        return *this;
    }

    RectangleExI& offsetLeft(int left)
    {
        left_ += left;
        updateWidth();
        return *this;
    }

    int right() const { return right_; }

    RectangleExI& setRight(int right)
    {
        right_ = right;
        updateWidth();
        return *this;
    }

    RectangleExI& offsetRight(int right)
    {
        right_ += right;
        updateWidth();
        return *this;
    }

    int top() const { return top_; }

    RectangleExI& setTop(int top)
    {
        top_ = top;
        updateHeight();
        return *this;
    }

    RectangleExI& offsetTop(int top)
    {
        top_ += top;
        updateHeight();
        return *this;
    }

    int bottom() const { return bottom_; }

    RectangleExI& setBottom(int bottom)
    {
        bottom_ = bottom;
        updateHeight();
        return *this;
    }

    RectangleExI& offsetBottom(int bottom)
    {
        bottom_ += bottom;
        updateHeight();
        return *this;
    }

    int width() const { return width_; }

    RectangleExI& resizeWidthRight(int width)
    {
        width_ = width;
        updateRight();
        return *this;
    }

    RectangleExI& resizeWidthLeft(int width)
    {
        width_ = width;
        updateLeft();
        return *this;
    }

    int height() const { return height_; }

    RectangleExI& resizeHeightBottom(int height)
    {
        height_ = height;
        updateBottom();
        return *this;
    }

    RectangleExI& resizeHeightTop(int height)
    {
        height_ = height;
        updateTop();
        return *this;
    }

    Vector2I topLeft() const
    {
        return Vector2I{
            left_,
            top_};
    }

    Vector2I bottomRight() const
    {
        return Vector2I{
            right_,
            bottom_};
    }

    Vector2I center() const
    {
        return Vector2I{
            (left_ + (width_ / 2)),
            (top_ + (height_ / 2))};
    }

    Rectangle rectangle() const
    {
        return Rectangle{
            static_cast<float>(left_),
            static_cast<float>(top_),
            static_cast<float>(width_),
            static_cast<float>(height_)};
    }

private:
    void updateWidth()
    {
        width_ = right_ - left_ + 1;
    }

    void updateHeight()
    {
        height_ = bottom_ - top_ + 1;
    }

    void updateLeft()
    {
        left_ = right_ - width_ + 1;
    }

    void updateTop()
    {
        top_ = bottom_ - height_ + 1;
    }

    void updateRight()
    {
        right_ = left_ + width_ - 1;
    }

    void updateBottom()
    {
        bottom_ = top_ + height_ - 1;
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
//=====================================

// Functions
//=====================================
// in fullscreen mode we can't use GetScreenWidth/Height, so make a function that gets the right data for each mode
inline Vector2 GetDisplaySize()
{
    if (IsWindowFullscreen())
    {
        return Vector2{static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};
    }
    else
    {
        return Vector2{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
    }
}

inline int Vector2Sum(Vector2I const& V)
{
    return abs(V.x) + abs(V.y);
}

inline Vector2 GetMainDirection(Vector2 const& from, Vector2 const& to)
{
    Vector2 v{Vector2Subtract(to, from)};

    (abs(v.x) > abs(v.y))
        ? (v.y = 0)
        : (v.x = 0);

    return Vector2Normalize(v);
}

inline Rectangle GetWindowRec()
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

inline bool IsKeyPressedRepeat(int key, float delay, float tick)
{
    static float dtDelay{0};
    static float dtTick{tick};

    if (IsKeyDown(key))
    {
        // Start measuring delay
        dtDelay += GetFrameTime();

        // If delay exceeded
        if (dtDelay > delay)
        {
            // Avoid huge dtDelay value
            dtDelay = delay;

            // Start measuring ticks
            dtTick += GetFrameTime();

            // If tick exceeded (includes initial trigger)
            if (dtTick > tick)
            {
                // Reset tick
                dtTick -= tick;

                // Confirm exceeded tick
                return true;
            }
        }
    }
    else
    {
        dtDelay = 0;
        dtTick = tick;
    }

    return IsKeyPressed(key);
};
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

// Check whether two given integer vectors are equal
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

inline bool CheckCollisionPointRec(Vector2I point, RectangleExI const& rec)
{
    return (
        (point.x >= rec.left())
        && (point.x <= rec.right())
        && (point.y >= rec.top())
        && (point.y <= rec.bottom()));
}

inline bool CheckCollisionPointRec(Vector2 point, RectangleEx rec)
{
    return CheckCollisionPointRec(point, rec.rectangle());
}
//=====================================

// Operator Overloads
//=====================================
inline Vector2& operator+=(Vector2& lhs, Vector2 const& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

inline Vector2& operator-=(Vector2& lhs, Vector2 const& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

inline bool operator==(Vector2 lhs, Vector2 rhs)
{
    return Vector2Equals(lhs, rhs);
}

inline Vector2I& operator+=(Vector2I& lhs, Vector2I const& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

inline Vector2I& operator-=(Vector2I& lhs, Vector2I const& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

inline bool operator==(Vector2I const& lhs, Vector2I const& rhs)
{
    return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}
//=====================================

// Hash overloads
//=====================================
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
