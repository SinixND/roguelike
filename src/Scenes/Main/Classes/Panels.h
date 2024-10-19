#ifndef IG20240531174000
#define IG20240531174000

#include "Objects.h"
#include "raylibEx.h"

class Panels
{
    //* Panels (sorted by descending priority)
    float panelBorderWeight{};

    //* Info (right), Number: chars per line
    float infoPanelWidth{};

    //* Status (top), Number: lines to print
    float statusPanelHeight{};

    //* Log (bottom)
    float logPanelHeight{};

public:
    RectangleEx tileInfo{};
    RectangleEx info{};
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
