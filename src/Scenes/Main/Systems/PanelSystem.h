#ifndef IG20241028002927
#define IG20241028002927

#include "Hero.h"
#include "Objects.h"
#include "Panels.h"
#include "raylibEx.h"

namespace PanelSystem
{
    void init(Panels& panels);

    void drawGameInfoPanelContent(
        Panels const& panels,
        int level);

    void drawHeroInfoPanelContent(
        Panels const& panels,
        Hero const& hero);

    void drawTileInfoPanelContent(
        Panels const& panels,
        Objects const& objects,
        Vector2I const& cursorPosition);

    void drawLogPanelContent(Panels const& panels);

    void drawPanelBorders(Panels const& panels);
}

#endif