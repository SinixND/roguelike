#ifndef IG20240203171045
#define IG20240203171045

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <utility>

//* Typedefs

class RectangleEx
{
    float left_{};
    float top_{};
    float right_{};
    float bottom_{};
    float width_{};
    float height_{};

public:
    RectangleEx() = default;

    RectangleEx(
        float left,
        float top,
        float width = 0,
        float height = 0
    )
        : left_( left )
        , top_( top )
        , right_( left + width )
        , bottom_( top + height )
        , width_( width )
        , height_( height )
    {
        validate();
    }

    RectangleEx(
        Vector2 const& topLeft,
        Vector2 const& bottomRight = { 0, 0 }
    )
        : left_( topLeft.x )
        , top_( topLeft.y )
        , right_( bottomRight.x )
        , bottom_( bottomRight.y )
        , width_( bottomRight.x - topLeft.x )
        , height_( bottomRight.y - topLeft.y )
    {
        validate();
    }

    RectangleEx(
        Vector2 const& center,
        float width,
        float height
    )
        : left_( center.x - ( 0.5f * width ) )
        , top_( center.y - ( 0.5f * height ) )
        , right_( center.x + ( 0.5f * width ) )
        , bottom_( center.y + ( 0.5f * height ) )
        , width_( width )
        , height_( height )
    {
        validate();
    }

    explicit RectangleEx( Rectangle const& rectangle )
        : left_( rectangle.x )
        , top_( rectangle.y )
        , right_( rectangle.x + rectangle.width )
        , bottom_( rectangle.y + rectangle.height )
        , width_( rectangle.width )
        , height_( rectangle.height )
    {
        validate();
    }

    float left() const
    {
        return left_;
    }

    RectangleEx& setLeft( float left )
    {
        left_ = left;
        updateWidth();
        return *this;
    }

    RectangleEx& offsetLeft( float left )
    {
        left_ += left;
        updateWidth();
        return *this;
    }

    float right() const
    {
        return right_;
    }

    RectangleEx& setRight( float right )
    {
        right_ = right;
        updateWidth();
        return *this;
    }

    RectangleEx& offsetRight( float right )
    {
        right_ += right;
        updateWidth();
        return *this;
    }

    float top() const
    {
        return top_;
    }

    RectangleEx& setTop( float top )
    {
        top_ = top;
        updateHeight();
        return *this;
    }

    RectangleEx& offsetTop( float top )
    {
        top_ += top;
        updateHeight();
        return *this;
    }

    float bottom() const
    {
        return bottom_;
    }

    RectangleEx& setBottom( float bottom )
    {
        bottom_ = bottom;
        updateHeight();
        return *this;
    }

    RectangleEx& offsetBottom( float bottom )
    {
        bottom_ += bottom;
        updateHeight();
        return *this;
    }

    float width() const
    {
        return width_;
    }

    RectangleEx& resizeWidthRight( float width )
    {
        width_ = width;
        updateRight();
        return *this;
    }

    RectangleEx& resizeWidthLeft( float width )
    {
        width_ = width;
        updateLeft();
        return *this;
    }

    float height() const
    {
        return height_;
    }

    RectangleEx& resizeHeightBottom( float height )
    {
        height_ = height;
        updateBottom();
        return *this;
    }

    RectangleEx& resizeHeightTop( float height )
    {
        height_ = height;
        updateTop();
        return *this;
    }

    Vector2 topLeft() const
    {
        return Vector2{
            left_,
            top_
        };
    }

    Vector2 bottomRight() const
    {
        return Vector2{
            right_,
            bottom_
        };
    }

    Vector2 center() const
    {
        return Vector2{
            ( left_ + ( 0.5f * width_ ) ),
            ( top_ + ( 0.5f * height_ ) )
        };
    }

    Rectangle rectangle() const
    {
        return Rectangle{
            left_,
            top_,
            width_,
            height_
        };
    }

private:
    void updateWidth()
    {
        width_ = right_ - left_;
    }

    void updateHeight()
    {
        height_ = bottom_ - top_;
    }

    void updateLeft()
    {
        left_ = right_ - width_;
    }

    void updateTop()
    {
        top_ = bottom_ - height_;
    }

    void updateRight()
    {
        right_ = left_ + width_;
    }

    void updateBottom()
    {
        bottom_ = top_ + height_;
    }

    void validate()
    {
        if ( right_ < left_ )
        {
            float temp{ right_ };
            right_ = left_;
            left_ = temp;
        }

        if ( bottom_ < top_ )
        {
            float temp{ bottom_ };
            bottom_ = top_;
            top_ = temp;
        }
    }
};

struct Vector2I
{
    int x;
    int y;
};

class RectangleExI
{
    int left_{};
    int top_{};
    int right_{};
    int bottom_{};
    int width_{};
    int height_{};

public:
    RectangleExI() = default;

    RectangleExI(
        int left,
        int top,
        int width = 1,
        int height = 1
    )
        : left_( left )
        , top_( top )
        , right_( left + width - 1 )
        , bottom_( top + height - 1 )
        , width_( width )
        , height_( height )
    {
        validate();
    }

    RectangleExI(
        Vector2I const& topLeft,
        Vector2I const& bottomRight = { 0, 0 }
    )
        : left_( topLeft.x )
        , top_( topLeft.y )
        , right_( bottomRight.x )
        , bottom_( bottomRight.y )
        , width_( bottomRight.x - topLeft.x + 1 )
        , height_( bottomRight.y - topLeft.y + 1 )
    {
        validate();
    }

    //* Only odd values for width/height
    RectangleExI(
        Vector2I const& center,
        int width,
        int height
    )
        : left_( center.x - ( width / 2 ) )
        , top_( center.y - ( height / 2 ) )
        , right_( center.x + ( width / 2 ) )
        , bottom_( center.y + ( height / 2 ) )
        , width_( width )
        , height_( height )
    {
        validate();
#if defined( DEBUG )
        assert( ( width % 2 ) && ( height % 2 ) && "[ERROR] Width or height invalid; Must be odd value!" );
#endif
    }

    int left() const
    {
        return left_;
    }

    RectangleExI& setLeft( int left )
    {
        left_ = left;
        updateWidth();
        return *this;
    }

    RectangleExI& offsetLeft( int left )
    {
        left_ += left;
        updateWidth();
        return *this;
    }

    int right() const
    {
        return right_;
    }

    RectangleExI& setRight( int right )
    {
        right_ = right;
        updateWidth();
        return *this;
    }

    RectangleExI& offsetRight( int right )
    {
        right_ += right;
        updateWidth();
        return *this;
    }

    int top() const
    {
        return top_;
    }

    RectangleExI& setTop( int top )
    {
        top_ = top;
        updateHeight();
        return *this;
    }

    RectangleExI& offsetTop( int top )
    {
        top_ += top;
        updateHeight();
        return *this;
    }

    int bottom() const
    {
        return bottom_;
    }

    RectangleExI& setBottom( int bottom )
    {
        bottom_ = bottom;
        updateHeight();
        return *this;
    }

    RectangleExI& offsetBottom( int bottom )
    {
        bottom_ += bottom;
        updateHeight();
        return *this;
    }

    int width() const
    {
        return width_;
    }

    RectangleExI& resizeWidthRight( int width )
    {
        width_ = width;
        updateRight();
        return *this;
    }

    RectangleExI& resizeWidthLeft( int width )
    {
        width_ = width;
        updateLeft();
        return *this;
    }

    int height() const
    {
        return height_;
    }

    RectangleExI& resizeHeightBottom( int height )
    {
        height_ = height;
        updateBottom();
        return *this;
    }

    RectangleExI& resizeHeightTop( int height )
    {
        height_ = height;
        updateTop();
        return *this;
    }

    Vector2I topLeft() const
    {
        return Vector2I{
            left_,
            top_
        };
    }

    Vector2I bottomRight() const
    {
        return Vector2I{
            right_,
            bottom_
        };
    }

    Vector2I center() const
    {
        return Vector2I{
            ( left_ + ( width_ / 2 ) ),
            ( top_ + ( height_ / 2 ) )
        };
    }

    Rectangle rectangle() const
    {
        return Rectangle{
            static_cast<float>( left_ ),
            static_cast<float>( top_ ),
            static_cast<float>( width_ ),
            static_cast<float>( height_ )
        };
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

    void validate()
    {
        assert( width_ && height_ && "Width or height < 1" );

        if ( right_ < left_ )
        {
            int temp{ right_ };
            right_ = left_;
            left_ = temp;

            updateWidth();
        }

        if ( bottom_ < top_ )
        {
            int temp{ bottom_ };
            bottom_ = top_;
            top_ = temp;

            updateHeight();
        }
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

//* Function Overloads

//* Add two vectors (v1 + v2)
RMAPI Vector2I Vector2Add( Vector2I const& v1, Vector2I const& v2 )
{
    return Vector2I{
        v1.x + v2.x,
        v1.y + v2.y
    };
}

RMAPI Vector2I Vector2AddValue( Vector2I const& v1, int value )
{
    return Vector2I{
        v1.x + value,
        v1.y + value
    };
}

//* Subtract two vectors (v1 - v2)
RMAPI Vector2I Vector2Subtract( Vector2I const& v1, Vector2I const& v2 )
{
    return Vector2I{
        v1.x - v2.x,
        v1.y - v2.y
    };
}

RMAPI Vector2I Vector2SubtractValue( Vector2I const& v1, int value )
{
    return Vector2I{
        v1.x - value,
        v1.y - value
    };
}

//* Scale vector (multiply by value)
RMAPI Vector2 Vector2Scale( Vector2I const& v, float scale )
{
    return Vector2{
        v.x * scale,
        v.y * scale
    };
}

RMAPI Vector2I Vector2Scale( Vector2I const& v, int scale )
{
    return Vector2I{
        v.x * scale,
        v.y * scale
    };
}

//* Check whether two given integer vectors are equal
RMAPI bool Vector2Equals( Vector2I const& v1, Vector2I const& v2 )
{
    return ( ( v1.x == v2.x ) && ( v1.y == v2.y ) );
}

RMAPI Vector2 Vector2Round( Vector2 v )
{
    v.x = roundf( v.x );
    v.y = roundf( v.y );
    return v;
}

RMAPI Vector2I Vector2TransMove( Matrix2x2I const& M, Vector2I const& v )
{
    return Vector2I{
        ( ( M.m11 * v.x ) + ( M.m12 * v.y ) ),
        ( ( M.m21 * v.x ) + ( M.m22 * v.y ) )
    };
}

RMAPI Vector2 Vector2TransMove( Matrix2x2 const& M, Vector2 const& v )
{
    return Vector2{
        ( ( M.m11 * v.x ) + ( M.m12 * v.y ) ),
        ( ( M.m21 * v.x ) + ( M.m22 * v.y ) )
    };
}

RMAPI Vector2I Vector2Negate( Vector2I const& v )
{
    return Vector2I{ -v.x, -v.y };
}

RMAPI int Vector2Distance( Vector2I const& start, Vector2I const& target )
{
    return ( abs( target.x - start.x ) + abs( target.y - start.y ) );
}

RMAPI Vector2I Vector2Normalize( Vector2I const& v )
{
    return Vector2I{
        ( ( v.x ) ? v.x / abs( v.x ) : v.x ),
        ( ( v.y ) ? v.y / abs( v.y ) : v.y )
    };
}

RMAPI int Vector2Length( Vector2I const& v )
{
    return ( abs( v.x ) + abs( v.y ) );
}

inline bool CheckCollisionPointRec(
    Vector2I const& point,
    RectangleExI const& rec
)
{
    return (
        ( point.x >= rec.left() )
        && ( point.x <= rec.right() )
        && ( point.y >= rec.top() )
        && ( point.y <= rec.bottom() )
    );
}

inline bool CheckCollisionPointRec(
    Vector2 const& point,
    RectangleEx const& rec
)
{
    return CheckCollisionPointRec(
        point,
        rec.rectangle()
    );
}

//* 0 = colinear, >0 (positive) = CW, <0 (negative) = CCW
inline int GetOrientation(
    Vector2 const& p1,
    Vector2 const& p2,
    Vector2 const& p3
)
{
    //* https://www.geeksforgeeks.org/orientation-3-ordered-points/
    float n{ ( p2.y - p1.y ) * ( p3.x - p2.x ) - ( p2.x - p1.x ) * ( p3.y - p2.y ) };
    return n / abs( n );
}

inline bool CheckCollisionLines(
    Vector2 const& a1,
    Vector2 const& b1,
    Vector2 const& c2,
    Vector2 const& d2
)
{
    //* https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
    //* REQUIREMENT:
    //*  - Orientation of a1-b1-c2 and a1-b1-d2 differs
    //*  AND
    //*  - Orientation of c2-d2-a1 and c2-d2-b1 differs
    //* CONCLUSION: No collision if
    //*  - one orientation is colinear (0) := touch but no intersection
    //*  OR
    //*  - one orientation pair is equal

    int abc{ GetOrientation( a1, b1, c2 ) };
    int abd{ GetOrientation( a1, b1, d2 ) };

    int cda{ GetOrientation( c2, d2, a1 ) };
    int cdb{ GetOrientation( c2, d2, b1 ) };

    if ( //* Colinear orientations
        !( abc * abd * cda * cdb )
        //* No intersection
        || ( abc == abd )
        || ( cda == cdb )
    )
    {
        return false;
    }

    return true;
}

inline bool CheckCollisionLineRec(
    Vector2 const& a1,
    Vector2 const& b1,
    RectangleEx const& rectangle
)
{
    //* Check collsion of line with Rectangle diagonals
    if ( CheckCollisionLines(
             a1,
             b1,
             rectangle.topLeft(),
             rectangle.bottomRight()
         )
         || CheckCollisionLines(
             a1,
             b1,
             Vector2(
                 {

                     rectangle.left(),
                     rectangle.bottom()
                 }
             ),
             Vector2(
                 { rectangle.right(),
                   rectangle.top() }
             )
         ) )
    {
        return true;
    }

    return false;
}

//* Functions

//* In fullscreen mode we can't use GetScreenWidth/Height, so make a function that gets the right data for each mode
inline Vector2 GetDisplaySize()
{
    if ( IsWindowFullscreen() )
    {
        return Vector2{ static_cast<float>( GetMonitorWidth( GetCurrentMonitor() ) ), static_cast<float>( GetMonitorHeight( GetCurrentMonitor() ) ) };
    }

    else
    {
        return Vector2{ static_cast<float>( GetRenderWidth() ), static_cast<float>( GetRenderHeight() ) };
    }
}

inline int Vector2Sum( Vector2I const& v )
{
    return abs( v.x ) + abs( v.y );
}

inline Vector2 Vector2MainDirection( Vector2 v )
{
    ( abs( v.x ) > abs( v.y ) )
        ? ( v.y = 0 )
        : ( v.x = 0 );

    return Vector2Normalize( v );
}

inline Vector2I Vector2MainDirection( Vector2I v )
{
    ( abs( v.x ) > abs( v.y ) )
        ? ( v.y = 0 )
        : ( v.x = 0 );

    return Vector2Normalize( v );
}

inline Vector2 Vector2MainDirection( Vector2 const& from, Vector2 const& to )
{
    return Vector2MainDirection(
        Vector2Subtract(
            to,
            from
        )
    );
}

inline Vector2I Vector2MainDirection( Vector2I const& from, Vector2I const& to )
{
    return Vector2MainDirection(
        Vector2Subtract(
            to,
            from
        )
    );
}

inline Vector2 Vector2OffDirection( Vector2 v )
{
    ( abs( v.x ) < abs( v.y ) )
        ? ( v.y = 0 )
        : ( v.x = 0 );

    return Vector2Normalize( v );
}

inline Vector2I Vector2OffDirection( Vector2I v )
{
    ( abs( v.x ) < abs( v.y ) )
        ? ( v.y = 0 )
        : ( v.x = 0 );

    return Vector2Normalize( v );
}

inline Vector2 Vector2OffDirection( Vector2 const& from, Vector2 const& to )
{
    return Vector2OffDirection(
        Vector2Subtract(
            to,
            from
        )
    );
}

inline Vector2I Vector2OffDirection( Vector2I const& from, Vector2I const& to )
{
    return Vector2OffDirection(
        Vector2Subtract(
            to,
            from
        )
    );
}

inline Vector2I Vector2Swap( Vector2I v )
{
    std::swap( v.x, v.y );

    return v;
}

inline Rectangle GetWindowRec()
{
    return Rectangle{
        0,
        0,
        static_cast<float>( GetRenderWidth() ),
        static_cast<float>( GetRenderHeight() )
    };
}

inline Vector2 ConvertToVector2( Vector2I const& v )
{
    return Vector2{
        static_cast<float>( v.x ),
        static_cast<float>( v.y )
    };
}

inline Vector2I ConvertToVector2I( Vector2 const& v )
{
    return Vector2I{
        static_cast<int>( v.x ),
        static_cast<int>( v.y )
    };
}

inline Vector2I GetMin(
    Vector2I const& v1,
    Vector2I const& v2
)
{
    return Vector2I{
        ( v1.x < v2.x ? v1.x : v2.x ),
        ( v1.y < v2.y ? v1.y : v2.y )
    };
}

inline Vector2I GetMax(
    Vector2I const& v1,
    Vector2I const& v2
)
{
    return Vector2I{
        ( v1.x > v2.x ? v1.x : v2.x ),
        ( v1.y > v2.y ? v1.y : v2.y )
    };
}

inline void DrawTextExCentered(
    Font font,
    const char* text,
    RectangleEx rectangle,
    float yOffsetFromTop,
    float fontSize,
    float spacing,
    Color tint
)
{
    DrawTextEx(
        font,
        text,
        Vector2{
            rectangle.center().x - ( 0.5f * MeasureTextEx( font, text, fontSize, spacing ).x ),
            rectangle.center().y + yOffsetFromTop
        },
        fontSize,
        spacing,
        tint
    );
}

//* Operator Overloads
inline Vector2I& operator+=( Vector2I& lhs, Vector2I const& rhs )
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

inline Vector2I& operator-=( Vector2I& lhs, Vector2I const& rhs )
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

inline bool operator==( Vector2I const& lhs, Vector2I const& rhs )
{
    return ( ( lhs.x == rhs.x ) && ( lhs.y == rhs.y ) );
}

//* Overloads

//* hash
//* https://en.wikipedia.org/wiki/List_of_prime_numbers
int constexpr PRIME{ 2946901 };

template <>
struct std::hash<Vector2>
{
    size_t operator()( Vector2 const& v ) const
    {
        return ( PRIME + std::hash<float>()( v.x ) ) * PRIME + std::hash<float>()( v.y );
    }
};

template <>
struct std::hash<Vector2I>
{
    size_t operator()( Vector2I const& v ) const
    {
        return ( PRIME + std::hash<int>()( v.x ) ) * PRIME + std::hash<int>()( v.y );
    }
};

//* ostream
inline std::ostream& operator<<( std::ostream& os, Vector2 const& v )
{
    return os
           << "("
           << v.x
           << ", "
           << v.y
           << ")";
}

inline std::ostream& operator<<( std::ostream& os, Vector2I const& v )
{
    return os
           << "("
           << v.x
           << ", "
           << v.y
           << ")";
}
#endif
