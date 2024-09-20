#ifndef IG20240908213935
#define IG20240908213935

#include "DenseMap.h"
#include "Event.h"
#include "Position.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <cstddef>
#include <string>

class Objects
{
    snx::DenseMap<Vector2I, Position> positions_{};
    snx::DenseMap<Vector2I, RenderID> renderIDs_{};
    snx::DenseMap<Vector2I, std::string> tags_{};
    snx::DenseMap<Vector2I, Event> events_{};

public:
    // Access or create
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& tag,
        Event event);

    // void erase(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, Position> const& positions() const;
    Position const& position(Vector2I const& tilePosition) const;

    snx::DenseMap<Vector2I, std::string> const& tags() const;
    std::string const& tag(Vector2I const& tilePosition) const;

    snx::DenseMap<Vector2I, RenderID> const& renderIDs() const;
    RenderID renderID(Vector2I const& tilePosition) const;

    snx::DenseMap<Vector2I, Event> const& events() const;
    Event event(Vector2I const& tilePosition) const;

    size_t size() const;
};

#endif