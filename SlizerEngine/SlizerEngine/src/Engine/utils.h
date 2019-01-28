#include <vector>
#include <fstream>
#define SE_ERROR -1
#define SE_CONTINUE 0

static constexpr unsigned int SCR_WIDTH = 800;
static constexpr unsigned int SCR_HEIGHT = 600;

const std::vector<const char*> g_DeviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static std::vector<char> readFile(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) 
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

#ifdef _DEBUG
#define DEBUG_EXP(exp)              exp
#ifdef _WIN32
#define ASSERT_INVALID()            __debugbreak()
#else
#define ASSERT_INVALID()            { *(int*)0 = 0; }
#endif
#define ASSERT(exp, ...)            if (!(exp)) ASSERT_INVALID()
#define ASSERT_OR_ALERT(exp, ...)   ([&](){ if (!!(exp)) return (exp); ALERT("ALERT", __VA_ARGS__); exit(-1); })()

const std::vector<const char*> g_ValidationLayers = 
{
    "VK_LAYER_LUNARG_standard_validation"
};

#else

#define OUTPUT_MSG(...)             void(0)
#define LOG_ERROR(tag, ...)         void(0)
#define LOG_WARNING(tag, ...)       void(0)
#define LOG_INFO(tag, ...)          void(0)
#define ASSERT_INVALID()            void(0)
#define ALERT(...)                  void(0)
#define DEBUG_EXP(exp)
#define ASSERT(exp, ...)            void(0)
#define ASSERT_OR_ALERT(exp,...)    void(0)

#endif
