#ifndef IG20241028002927
#define IG20241028002927

struct Panels;

namespace PanelSystem
{
    [[nodiscard]]
    Panels const& init( Panels& panels );

    void drawPanelBorders( Panels const& panels );
}

#endif
