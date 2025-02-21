#ifndef IG20240908213935
#define IG20240908213935

#include "DenseMap.h"
#include "IdManager.h"
#include "raylibEx.h"
#include <string>

struct Vector2;
enum class RenderId;
enum class EventId;

class Objects
{
    snx::IdManager idManager_{};

public:
    snx::DenseMap<Vector2I, size_t> ids{};

    snx::DenseMap<size_t, Vector2> positions{};
    snx::DenseMap<size_t, RenderId> renderIds{};
    snx::DenseMap<size_t, std::string> names{};
    snx::DenseMap<size_t, std::string> actions{};
    snx::DenseMap<size_t, EventId> events{};

public:
    void insert(
        Vector2I const& tilePosition,
        std::string const& name,
        std::string const& action,
        RenderId renderId,
        EventId event
    );
};

#endif
