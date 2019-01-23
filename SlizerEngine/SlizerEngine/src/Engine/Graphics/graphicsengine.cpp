#include "graphicsengine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine/utils.h"

namespace Engine
{
    GraphicsEngine* GraphicsEngine::ms_GraphicsEngine = nullptr;

    GraphicsEngine::GraphicsEngine()
    {}

    GraphicsEngine::~GraphicsEngine()
    {
        ms_GraphicsEngine = nullptr;
    }

    int GraphicsEngine::Init()
    {
        if (!ms_GraphicsEngine)
        {
            ms_GraphicsEngine = new GraphicsEngine();
        }

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return SE_ERROR;
        }

        return SE_CONTINUE;
    }

    int GraphicsEngine::Update(GLFWwindow* window)
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        return SE_CONTINUE;
    }

    GraphicsEngine& GraphicsEngine::GetInstance()
    {
        if (!ms_GraphicsEngine)
        {
            ms_GraphicsEngine = new GraphicsEngine();
        }

        return *ms_GraphicsEngine;
    }
}
