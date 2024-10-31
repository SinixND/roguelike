#ifndef IG20241028002927
#define IG20241028002927

struct Panels;
struct Hero;
struct ObjectSoA;
struct Vector2I;

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
        ObjectSoA const& objects,
        Vector2I const& cursorPosition);

    void drawLogPanelContent(Panels const& panels);

    void drawPanelBorders(Panels const& panels);
}

#endif