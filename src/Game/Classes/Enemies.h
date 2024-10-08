#ifndef IG20240909134010
#define IG20240909134010

#include "AI.h"
#include "DenseMap.h"
#include "Energy.h"
#include "GameCamera.h"
#include "IdManager.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <cstddef>

// SoA class -> provides setters/getters to allow entity-like usage
class Enemies
{
    snx::IdManager idManager_{};

    snx::DenseMap<Vector2I, size_t> ids_{};
    snx::DenseMap<size_t, RenderID> renderIDs_{};
    snx::DenseMap<size_t, Movement> movements_{};
    snx::DenseMap<size_t, Energy> energies_{};
    snx::DenseMap<size_t, Position> positions_{};
    snx::DenseMap<size_t, AI> ais_{};

public:
    // Pass heroPosition twice to get random position
    void create(
        Map const& map,
        RenderID enemyID,
        Vector2I tilePosition = Vector2I{0, 0});

    void init(
        int mapLevel,
        Map const& map);

    bool regenerate();

    // Checks consecutively, persistant across game loop iterations
    bool checkForAction(
        Map const& map,
        Vector2I const& heroPosition,
        GameCamera const& gameCamera);

    void update(
        Map const& map,
        Position const& heroPosition);

    // Getters / Setters
    snx::DenseMap<size_t, Movement> const& movements() const;
    Movement const& movement(size_t id) const;
    Movement& movement(size_t id);

    snx::DenseMap<size_t, Energy> const& energies() const;
    Energy const& energy(size_t id) const;
    Energy& energy(size_t id);

    snx::DenseMap<size_t, Position> const& positions() const;
    Position const& position(size_t id) const;
    Position& position(size_t id);

    snx::DenseMap<size_t, RenderID> const& renderIDs() const;
    RenderID renderID(size_t id) const;

    snx::DenseMap<size_t, AI> const& ais() const;
    AI const& ai(size_t id) const;
    AI& ai(size_t id);

    snx::DenseMap<Vector2I, size_t> const& ids() const;
    size_t id(Vector2I const& tilePosition) const;

private:
    Vector2I getRandomPosition(Tiles const& tiles);

    void insert(
        size_t id,
        RenderID renderID,
        Movement const& movement,
        Energy const& energy,
        int scanRange,
        Vector2I const& enemyPosition);
};

#endif
