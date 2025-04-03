#include "Panel.h"

Panel::Panel( RectangleEx rectangle )
    : rectangle_{ rectangle }
{
}

Panel::Panel(
    RectangleEx rectangle,
    float margin,
    float padding
)
    : rectangle_{ rectangle }
    , margin_{ margin }
    , padding_{ padding }
{
}

void Panel::setRectangle( RectangleEx rectangle )
{
    rectangle_ = rectangle;
}

void Panel::setOuter(
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

RectangleEx const& Panel::box() const
{
    return rectangle_;
}

RectangleEx Panel::inner() const
{
    return RectangleEx{
        rectangle_.left() + padding_,
        rectangle_.top() + padding_,
        rectangle_.width() - 2 * padding_,
        rectangle_.height() - 2 * padding_
    };
}

RectangleEx Panel::outer() const
{
    return RectangleEx{
        rectangle_.left() - margin_,
        rectangle_.top() - margin_,
        rectangle_.width() + 2 * margin_,
        rectangle_.height() + 2 * margin_
    };
}

void Panel::drawBackground( Color const& color ) const
{
    DrawRectangleRec(
        box().rectangle(),
        color
    );
}

void Panel::drawBorder( float borderWidth ) const
{
    DrawRectangleLinesEx(
        box().rectangle(),
        borderWidth,
        Color{ 25, 25, 25, 255 }
    );
}

