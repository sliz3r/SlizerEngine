#include "engine.h"
#include "Graphics/graphicsengine.h"
#include "utils.h"
#include <iostream>

namespace Engine
{  
   //WIP
   // settings
    static constexpr unsigned int SCR_WIDTH = 800;
    static constexpr unsigned int SCR_HEIGHT = 600;
    void processInput(GLFWwindow *window);
   //

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
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


        // glfw window creation
        // --------------------
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Slizer Engine - 3D Vulkan Engine", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            return SE_ERROR;
        }

        if (m_GraphicsEngine->Init() == SE_ERROR)
        {
            return SE_ERROR;
        }
        
        return Update();
    }

    int Engine::Update()
    {
        int returnValue = 0;
        while (!glfwWindowShouldClose(window) && returnValue == SE_CONTINUE)
        {
            // input
            // -----
            processInput(window);

            //Engine graphics
            returnValue = m_GraphicsEngine->Update(window);

            glfwPollEvents();
        }
        return returnValue;
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}
