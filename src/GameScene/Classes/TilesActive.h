#ifndef IG20240907174210
#define IG20240907174210

#include "DenseMap.h"
#include "Position.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <functional>
#include <raylib.h>
#include <string>
#include <unordered_set>

class TilesActive
{
    snx::DenseMap<Vector2I, Position> positions_{};
    snx::DenseMap<Vector2I, RenderID> renderIDs_{};
    snx::DenseMap<Vector2I, std::string> tags_{};
    snx::DenseMap<Vector2I, VisibilityID> visibilityIDs_{};
    std::unordered_set<Vector2I> isSolids_{};
    std::unordered_set<Vector2I> isOpaques_{};
    snx::DenseMap<Vector2I, std::function<void()>> events_{};

    RectangleExI mapSize_{};

private:
    void updateMapSize(Vector2I const& tilePosition);

public:
    // Overwrites
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& tag,
        std::function<void()> event,
        VisibilityID visibilityID = VisibilityID::invisible,
        bool isSolid = false,
        bool isOpaque = false);

    // void erase(Vector2I const& tilePosition);

    size_t size();

    snx::DenseMap<Vector2I, Position>& positions();
    Position const& position(Vector2I const& tilePosition);

    std::string const& tag(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, RenderID>& renderIDs();
    RenderID renderID(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, VisibilityID>& visibilityIDs();
    VisibilityID visibilityID(Vector2I const& tilePosition);
    void setVisibilityID(Vector2I const& tilePosition, VisibilityID visibilityID);

    bool isSolid(Vector2I const& tilePosition);

    std::unordered_set<Vector2I> const& isOpaques();
    bool isOpaque(Vector2I const& tilePosition);

    RectangleExI mapSize();
};

#endif
