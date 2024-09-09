#ifndef IG20240909134010
#define IG20240909134010

#include "AI.h"
#include "DenseMap.h"
#include "Energy.h"
#include "IdManager.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <cstddef>

class Enemies
{
    snx::IdManager idManager_{};

    snx::DenseMap<size_t, RenderID> renderIDs_{};
    snx::DenseMap<size_t, AI> ais_{};
    snx::DenseMap<size_t, Movement> movements_{};
    snx::DenseMap<size_t, Energy> energies_{};
    snx::DenseMap<size_t, Position> positions_{};
    snx::DenseMap<Vector2I, size_t> ids_{};

    Vector2I getRandomPosition(Tiles& tiles);

    void insert(
        size_t id,
        RenderID renderID,
        AI const& ai,
        Movement const& movement,
        Energy const& energy,
        Tiles& tiles);

public:
    void create(RenderID enemyID, Tiles& tiles);
    void init(int mapLevel, Tiles& tiles);
    void update();

    snx::DenseMap<size_t, Movement>& movements();
    Movement& movement(size_t id);

    snx::DenseMap<size_t, Energy>& energies();
    Energy& energy(size_t id);

    snx::DenseMap<size_t, Position>& positions();
    Position& position(size_t id);

    snx::DenseMap<size_t, RenderID>& renderIDs();
    RenderID renderID(size_t id);

    snx::DenseMap<size_t, AI>& ais();
    AI& ai(size_t id);

    snx::DenseMap<Vector2I, size_t>& ids();
};

#endif
