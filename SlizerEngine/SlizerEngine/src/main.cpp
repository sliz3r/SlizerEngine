#include <iostream>
#include "Engine/engine.h"

int main()
{
    static Engine::Engine engine;
    int returnValue = engine.Init();
    return returnValue;
}

