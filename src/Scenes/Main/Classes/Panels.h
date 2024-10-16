#ifndef IG20240531174000
#define IG20240531174000

#include "Objects.h"
#include "raylibEx.h"

class Panels
{
public:
    RectangleEx tileInfo{};
    RectangleEx info_{};
    RectangleEx status{};
    RectangleEx log{};
    RectangleEx map{};

public:
    void init();

    void drawLogPanelContent() const;
    void drawTileInfoPanelContent(Objects const& objects, Vector2I const& cursorPosition) const;
    void drawStatusPanelContent(int level) const;

    void drawPanelBorders() const;
};

#endif
