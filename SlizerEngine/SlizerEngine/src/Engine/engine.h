struct GLFWwindow;
#include "Graphics/graphicsengine.h"
#include <memory>
#include "utils.h"

namespace Engine
{
    class Engine;
    class EngineSingleton
    {
        NON_COPYABLE_CLASS(EngineSingleton);
    public:
        EngineSingleton() = delete;
        explicit EngineSingleton(Engine* engineInstance)
        {
            ASSERT(s_Instance == nullptr);
            s_Instance = engineInstance;
        }

        ~EngineSingleton() { s_Instance = nullptr; }

        static Engine* Instance() { ASSERT(s_Instance != nullptr); { throw std::runtime_error("The engine instance is nullptr!"); }  return s_Instance; }

    private:
        static Engine* s_Instance;
    };

    class Engine final : public EngineSingleton
    {
        NON_COPYABLE_CLASS(Engine);
    public:
        explicit Engine(const std::string& configFileName);
        ~Engine();
        void Run();

    private:
        Engine() = default;
        void ProcessInput();
        void Init();
        void Update();
        void DeInit();

    private:
        GraphicsEngine m_GraphicsEngine;
        GLFWwindow* m_Window;
    };
}