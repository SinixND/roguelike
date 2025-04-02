#include "Panel.h"

void Panel::setRectangle( RectangleEx rectangle )
{
    rectangle_ = rectangle;
}

// void Panel::setMargin( float margin )
// {
//     margin_ = margin;
// }

// void Panel::setPadding( float padding )
// {
//     padding_ = padding;
// }

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

// void Panel::setInner(
//     RectangleEx inner,
//     float margin,
//     float padding
// )
// {
//     margin_ = margin;
//     padding_ = padding;
//
//     rectangle_ = RectangleEx{
//         inner.left() - padding,
//         inner.top() - padding,
//         inner.width() + 2 * padding,
//         inner.height() + 2 * padding,
//     };
// }

// float Panel::margin() const { return margin_; }
// float Panel::padding() const { return padding_; }

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

