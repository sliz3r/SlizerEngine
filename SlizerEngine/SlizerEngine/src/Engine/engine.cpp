#include "engine.h"
#include "utils.h"

namespace Engine
{  
    Engine* EngineSingleton::s_Instance = nullptr;

    Engine::Engine(const std::string& configFileName)
        : EngineSingleton(this)
    {}

    Engine::~Engine()
    {}

    void Engine::Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Slizer Engine - 3D Vulkan Engine", NULL, NULL);
        ASSERT(m_Window != nullptr);

        m_GraphicsEngine.Init(m_Window);
    }

    void Engine::Run()
    {
        Init();
        Update();
        DeInit();
    }

    void Engine::Update()
    {
        while (!glfwWindowShouldClose(m_Window))
        {
            ProcessInput();
            glfwPollEvents();

            //Graphics update
            m_GraphicsEngine.Update();
        }
    }

    void Engine::DeInit()
    {
        m_GraphicsEngine.DeInit();
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Engine::ProcessInput()
    {
        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_Window, true);
    }
}
