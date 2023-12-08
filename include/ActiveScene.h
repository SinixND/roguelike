#ifndef CURRENTSCENE_H_20231203203224
#define CURRENTSCENE_H_20231203203224

#include "Scene.h"
#include <mutex>

// included as an example of a parameterized macro
#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace snd
{
    class ActiveScene
    {
    public:
        static Scene& getScene();
        static void setScene(Scene& scene);

        // Singleton attributes
        //---------------------------------
        // get ActiveScene instance
        static ActiveScene* getInstance();
        //---------------------------------

    private:
        static Scene* scene_;

        // Singleton attributes
        //---------------------------------
        static ActiveScene* currentScene_;
        static std::mutex mutex_;

        // make ctor private
        ActiveScene();
        // make dtor private
        ~ActiveScene();

        DISALLOW_COPY_AND_ASSIGN(ActiveScene)
        //---------------------------------
    };
}
#endif
