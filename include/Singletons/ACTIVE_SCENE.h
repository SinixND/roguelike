#ifndef CURRENTSCENE_H_20231203203224
#define CURRENTSCENE_H_20231203203224

#include "Scene.h"
#include <type_traits>

// Included as an example of a parameterized macro
#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace snd
{
    class ACTIVE_SCENE
    {
    public:
        static inline Scene& getScene() { return *scene_; };
        static inline void setScene(Scene& scene) { scene_ = &scene; };

    private:
        static inline Scene* scene_{};

        // Singleton attributes
        //=================================
    public:
        // Get Singleton instance
        static inline ACTIVE_SCENE& get()
        {
            static ACTIVE_SCENE singleton;
            return singleton;
        };

    private:
        ACTIVE_SCENE() = default;
        ~ACTIVE_SCENE() = default;

        DISALLOW_COPY_AND_ASSIGN(ACTIVE_SCENE)
        //=================================
    };
}
#endif
