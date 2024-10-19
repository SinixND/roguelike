#ifndef IG20240531174000
#define IG20240531174000

#include "Hero.h"
#include "Objects.h"
#include "raylibEx.h"

class Panels
{
    float panelBorderWeight{};

    //* Status (top)
    float gameInfoPanelHeight{};

    //* Hero info (right)
    float heroInfoPanelWidth{};

    //* Log (bottom)
    float logPanelHeight{};

public:
    RectangleEx tileInfo{};
    RectangleEx heroInfo{};
    RectangleEx status{};
    RectangleEx log{};
    RectangleEx map{};

public:
    void init();

    void drawGameInfoPanelContent(int level) const;

    void drawHeroInfoPanelContent(Hero const& hero) const;
    
    void drawTileInfoPanelContent(
            Objects const& objects, 
            Vector2I const& cursorPosition) const;

    void drawLogPanelContent() const;

    void drawPanelBorders() const;
};

#endif
