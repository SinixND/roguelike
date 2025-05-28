#include "Panel.h"

Panel::Panel( RectangleEx const& rectangle )
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

void Panel::setRectangle( RectangleEx const& rectangle )
{
    rectangle_ = rectangle;
}

void Panel::setOuter(
    RectangleEx const& outer,
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

namespace PanelModule
{
    void drawBackground(
        Panel const& panel,
        Color const& color
    )
    {
        DrawRectangleRec(
            panel.box().rectangle(),
            color
        );
    }

    void drawBorder(
        Panel const& panel,
        float borderWidth
    )
    {
        DrawRectangleLinesEx(
            panel.box().rectangle(),
            borderWidth,
            Color{ 25, 25, 25, 255 }
        );
    }
}

