#ifndef IG20240426004239
#define IG20240426004239

class DebugMode
{
private:
    bool debugMode_{true};

public:
    auto operator()() const -> bool { return debugMode_; }
    void toggle() { debugMode_ = !debugMode_; }
};

#endif
