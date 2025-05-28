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

    explicit Panel( RectangleEx const& rectangle );

    Panel(
        RectangleEx rectangle,
        float margin,
        float padding
    );

    void setRectangle( RectangleEx const& rectangle );
    // void setMargin( float margin );
    // void setPadding( float padding );
    void setOuter(
        RectangleEx const& outer,
        float margin,
        float padding
    );

    // void setInner(
    //     RectangleEx inner,
    //     float margin,
    //     float padding
    // );

    // float margin() const;
    // float padding() const;

    RectangleEx const& box() const;
    RectangleEx inner() const;
    RectangleEx outer() const;
};

namespace PanelModule
{
    void drawBackground(
        Panel const& panel,
        Color const& color
    );

    void drawBorder(
        Panel const& panel,
        float borderWidth
    );
}
#endif
