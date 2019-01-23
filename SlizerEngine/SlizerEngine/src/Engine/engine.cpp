#include "engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils.h"

namespace Engine
{  
   //WIP
   // settings
    static constexpr unsigned int SCR_WIDTH = 800;
    static constexpr unsigned int SCR_HEIGHT = 600;
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow *window);
   //

    Engine* Engine::ms_Engine = nullptr;

    Engine::Engine()
    {}

    Engine::~Engine()
    {
        ms_Engine = nullptr;
    }

    int Engine::Init()
    {
        if (!ms_Engine)
        {
            ms_Engine = new Engine();
        }

        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

        // glfw window creation
        // --------------------
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return SE_ERROR;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (m_GraphicsEngine.Init() == SE_ERROR)
        {
            glfwTerminate();
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
            returnValue = m_GraphicsEngine.Update(window);

            glfwPollEvents();
        }

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return returnValue;
    }

    Engine& Engine::GetInstance()
    {
        if (!ms_Engine)
        {
            ms_Engine = new Engine();
        }

        return *ms_Engine;
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}
