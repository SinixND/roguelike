#ifndef IG20240513010437
#define IG20240513010437

#include "Singleton.h"

class DebugMode : public snx::Singleton<DebugMode>
{
private:
    static inline bool debugMode_{true};

public:
    static bool debugMode() { return instance().debugMode_; }
    static void toggleDebugMode() { instance().debugMode_ = !instance().debugMode_; }
};

#endif
