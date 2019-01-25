#include <vector>
#define SE_ERROR -1
#define SE_CONTINUE 0
static constexpr unsigned int SCR_WIDTH = 800;
static constexpr unsigned int SCR_HEIGHT = 600;

const std::vector<const char*> g_DeviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

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
