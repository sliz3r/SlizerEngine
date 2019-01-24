struct GLFWwindow;
class GraphicsEngine;

namespace Engine
{
    class  GraphicsEngine;
    class Engine
    {
    public:
        Engine();
        ~Engine();

        int Init();
        int Update();

    private:
        void processInput(GLFWwindow *window);

    private:
        GraphicsEngine* m_GraphicsEngine;
        GLFWwindow* window;
    };
}