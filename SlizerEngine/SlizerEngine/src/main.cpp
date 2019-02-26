#include <memory>
#include "Engine/engine.h"

int main()
{
    std::unique_ptr<Engine::Engine> engine = std::make_unique<Engine::Engine>("configPath");
    engine->Run();
    return 0;
}

