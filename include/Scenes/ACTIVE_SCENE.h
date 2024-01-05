#ifndef CURRENTSCENE_H_20231203203224
#define CURRENTSCENE_H_20231203203224

#include "Scene.h"
#include <mutex>
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
        // Get ActiveScene instance
        static inline ACTIVE_SCENE* getInstance()
        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (singleton_ == nullptr)
            {
                singleton_ = new ACTIVE_SCENE();
            }

            return singleton_;
        };

    private:
        static inline ACTIVE_SCENE* singleton_{nullptr};
        static inline std::mutex mutex_{};

        // Make ctor private
        ACTIVE_SCENE(){};
        // Make dtor private
        ~ACTIVE_SCENE(){};

        DISALLOW_COPY_AND_ASSIGN(ACTIVE_SCENE)
        //=================================
    };
}
#endif
