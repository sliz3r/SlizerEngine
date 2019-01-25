#include "engine.h"
#include "Graphics/graphicsengine.h"
#include "utils.h"
#include <iostream>

namespace Engine
{  
    Engine::Engine()
    {
        m_GraphicsEngine = new GraphicsEngine();
    }

    Engine::~Engine()
    {
        delete(m_GraphicsEngine);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    int Engine::Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Slizer Engine - 3D Vulkan Engine", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            return SE_ERROR;
        }

        if (m_GraphicsEngine->Init(window) == SE_ERROR)
        {
            return SE_ERROR;
        }
        
        return Update();
    }

    int Engine::Update()
    {
        int returnValue = SE_CONTINUE;
        while (!glfwWindowShouldClose(window) && returnValue == SE_CONTINUE)
        {
            processInput(window);

            //Graphics update
            returnValue = m_GraphicsEngine->Update();

            glfwPollEvents();
        }
        return returnValue;
    }

    void Engine::processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}
