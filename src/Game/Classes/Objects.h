#ifndef IG20240908213935
#define IG20240908213935

#include "DenseMap.h"
#include "Event.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <string>

//* SoA class
//* Containers need to stay in sync, therefore they are private and direct getters are provided to prevent individual inserts/deletions
class Objects
{
    snx::DenseMap<Vector2I, PositionComponent> positions_{};

    snx::DenseMap<Vector2I, RenderID> renderIDs_{};
    snx::DenseMap<Vector2I, std::string> tags_{};
    snx::DenseMap<Vector2I, Event> events_{};

public:
    //* Access or create
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& tag,
        Event event);

    snx::DenseMap<Vector2I, PositionComponent> const& getPositions() const;

    snx::DenseMap<Vector2I, RenderID> const& getRenderIDs() const;
    snx::DenseMap<Vector2I, RenderID>& getRenderIDs();

    snx::DenseMap<Vector2I, std::string> const& getTags() const;
    snx::DenseMap<Vector2I, std::string>& getTags();

    snx::DenseMap<Vector2I, Event> const& getEvents() const;
    snx::DenseMap<Vector2I, Event>& getEvents();
};

#endif