#ifndef IG20240426004239
#define IG20240426004239

class CheatMode
{
private:
    bool cheatMode_{true};

public:
    auto operator()() const -> bool { return cheatMode_; }
    void toggle() { cheatMode_ = !cheatMode_; }
};

#endif
