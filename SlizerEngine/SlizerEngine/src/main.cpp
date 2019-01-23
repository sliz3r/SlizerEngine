#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine/engine.h"

int main()
{
    static Engine::Engine engine;
    int returnValue = engine.Init();
    return returnValue;
}

