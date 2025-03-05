#ifndef IG20241028002927
#define IG20241028002927

struct Color;
class Panel;
struct SceneGamePanels;

namespace PanelSystem
{
    [[nodiscard]]
    SceneGamePanels const& init( SceneGamePanels& panels );

    void drawPanelBackground( Panel const& panel, Color const& color );

    void drawPanelBorder( Panel const& panel, float borderWidth );
}

#endif
