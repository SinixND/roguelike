#ifndef IG20240909151555
#define IG20240909151555

class AIComponent
{
    int scanRange_{};

public:
    AIComponent(int scanRange)
        : scanRange_(scanRange){};

    int scanRange() const;
};

#endif
