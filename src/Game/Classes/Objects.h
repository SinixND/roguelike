#ifndef IG20240908213935
#define IG20240908213935

#include "DenseMap.h"
#include "EventID.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <string>

class Objects
{
public:
    snx::DenseMap<Vector2I, PositionComponent> positions{};
    snx::DenseMap<Vector2I, RenderID> renderIDs{};
    snx::DenseMap<Vector2I, std::string> names{};
    snx::DenseMap<Vector2I, std::string> actions{};
    snx::DenseMap<Vector2I, EventID> events{};

public:
    //* Access or create
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& name,
        std::string const& action,
        EventID event);
};

#endif