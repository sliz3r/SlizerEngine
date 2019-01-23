struct GLFWwindow;
#include "Graphics/graphicsengine.h"

namespace Engine
{
    class  GraphicsEngine;
    class Engine
    {
    public:
        Engine();
        ~Engine();

        int Init();
        int Update();
        Engine& GetInstance();
        static bool IsInstanciated() { return ms_Engine != nullptr; }

    private:
        static Engine* ms_Engine;
        GraphicsEngine m_GraphicsEngine;
        GLFWwindow* window;
    };
}