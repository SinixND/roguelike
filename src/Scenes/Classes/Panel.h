#ifndef IG20250305143507
#define IG20250305143507

#include "raylibEx.h"

class Panel
{
    RectangleEx rectangle_{};
    //* Outer = rectancle + margin (= gap)
    float margin_{ 0 };
    //* Inner = rectancle - padding
    float padding_{ 0 };

public:
    Panel() = default;

    explicit Panel( RectangleEx rectangle )
        : rectangle_{ rectangle }
    {
    }

    Panel(
        RectangleEx rectangle,
        float margin,
        float padding
    )
        : rectangle_{ rectangle }
        , margin_{ margin }
        , padding_{ padding }
    {
    }

    void setRectangle( RectangleEx rectangle );
    void setMargin( float margin );
    void setPadding( float padding );
    void setOuter(
        RectangleEx outer,
        float margin,
        float padding
    );

    void setInner(
        RectangleEx inner,
        float margin,
        float padding
    );

    float margin() const;
    float padding() const;

    RectangleEx const& box() const;
    RectangleEx inner() const;
    RectangleEx outer() const;

    void drawBackground( Color const& color ) const;
    void drawBorder( float borderWidth ) const;
};

#endif
