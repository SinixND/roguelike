#ifndef IG20240909151555
#define IG20240909151555

class AI
{
    int scanRange_{};

public:
    AI(int scanRange)
        : scanRange_(scanRange){};

    int scanRange() const
    {
        return scanRange_;
    }
};

#endif
