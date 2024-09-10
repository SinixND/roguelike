#ifndef IG20240531174000
#define IG20240531174000

#include "Objects.h"
#include "raylibEx.h"

class Panels
{
    RectangleEx tileInfo_{};
    RectangleEx info_{};
    RectangleEx status_{};
    RectangleEx log_{};
    RectangleEx map_{};

public:
    RectangleEx const& tileInfo() const;
    RectangleEx const& info() const;
    RectangleEx const& status() const;
    RectangleEx const& log() const;
    RectangleEx const& map() const;

    void init();

    void drawLogPanelContent();
    void drawTileInfoPanelContent(Objects& objects, Vector2I const& cursorPosition);
    void drawPanelBorders();
};

#endif
