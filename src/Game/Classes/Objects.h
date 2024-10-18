#ifndef IG20240908213935
#define IG20240908213935

#include "DenseMap.h"
#include "Event.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <string>

//* SoA class
//* Its the clients responsibility to avoid desync caused by individual size modifications of DenseMaps (eg. insert, erase)
class Objects
{
public:
    snx::DenseMap<Vector2I, PositionComponent> positions{};
    snx::DenseMap<Vector2I, RenderID> renderIDs{};
    snx::DenseMap<Vector2I, std::string> tags{};
    snx::DenseMap<Vector2I, Event> events{};

public:
    //* Access or create
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& tag,
        Event event);
};

#endif