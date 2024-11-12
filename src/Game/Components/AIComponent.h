#ifndef IG20240909151555
#define IG20240909151555

class PositionComponent;
class MovementComponent;
class EnergyComponent;
class Map;
struct Vector2I;
class GameCamera;

class AIComponent
{
    int scanRange_{};

public:
    AIComponent(int scanRange)
        : scanRange_(scanRange){};

public:
    int scanRange() const;
};

#endif
