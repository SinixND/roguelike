#ifndef IG20240531174000
#define IG20240531174000

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

    void setRectangle( RectangleEx rectangle )
    {
        rectangle_ = rectangle;
    }

    void setMargin( float margin )
    {
        margin_ = margin;
    }

    void setPadding( float padding )
    {
        padding_ = padding;
    }

    void setOuter(
        RectangleEx outer,
        float margin,
        float padding
    )
    {
        margin_ = margin;
        padding_ = padding;

        rectangle_ = RectangleEx{
            outer.left() + margin,
            outer.top() + margin,
            outer.width() - 2 * margin,
            outer.height() - 2 * margin,
        };
    }

    void setInner(
        RectangleEx inner,
        float margin,
        float padding
    )
    {
        margin_ = margin;
        padding_ = padding;

        rectangle_ = RectangleEx{
            inner.left() - padding,
            inner.top() - padding,
            inner.width() + 2 * padding,
            inner.height() + 2 * padding,
        };
    }

    float margin() const { return margin_; }
    float padding() const { return padding_; }

    RectangleEx const& box() const
    {
        return rectangle_;
    }

    RectangleEx inner() const
    {
        return RectangleEx{
            rectangle_.left() + padding_,
            rectangle_.top() + padding_,
            rectangle_.width() - 2 * padding_,
            rectangle_.height() - 2 * padding_
        };
    }

    RectangleEx outer() const
    {
        return RectangleEx{
            rectangle_.left() - margin_,
            rectangle_.top() - margin_,
            rectangle_.width() + 2 * margin_,
            rectangle_.height() + 2 * margin_
        };
    }
};

struct Panels
{
    Panel map;
    Panel status;
    Panel log;
    Panel info;
};

#endif
