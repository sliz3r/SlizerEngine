struct GLFWwindow;

namespace Engine
{
    class GraphicsEngine
    {
    public:
        GraphicsEngine();
        ~GraphicsEngine();

        int Init();
        int Update(GLFWwindow* window);
        GraphicsEngine& GetInstance();
        static bool IsInstanciated() { return ms_GraphicsEngine != nullptr; }

    private:
        static GraphicsEngine* ms_GraphicsEngine;
    };
}