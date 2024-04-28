#ifndef IG20240426004239
#define IG20240426004239

class DebugMode
{
private:
    bool debugMode_{true};

public:
    bool operator()() const { return debugMode_; }
    void toggle() { debugMode_ = !debugMode_; }
};

#endif