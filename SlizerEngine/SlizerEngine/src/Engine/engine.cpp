#include "engine.h"
#include "utils.h"

namespace Engine
{  
    Engine* EngineSingleton::s_Instance = nullptr;

    Engine::Engine(const std::string& configFileName)
        : EngineSingleton(this)
    {}

    Engine::~Engine()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Engine::Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Slizer Engine - 3D Vulkan Engine", NULL, NULL);
        if (m_Window == NULL)
        {
            throw std::runtime_error("failed to create Window Instance!");
        }

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

            //Graphics update
            m_GraphicsEngine.Update();

            glfwPollEvents();
        }
    }

    void Engine::DeInit()
    {
        m_GraphicsEngine.DeInit();
    }

    void Engine::ProcessInput()
    {
        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_Window, true);
    }
}
