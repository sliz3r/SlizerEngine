#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Engine
{
    class GraphicsEngine
    {
    public:
        GraphicsEngine();
        ~GraphicsEngine();

        int Init();
        int Update(GLFWwindow* window);

    private:
        int CreateVulkanInstance();

#ifdef _DEBUG
        bool CheckValidationLayerSupport();
        void SetupDebugMessenger();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
#endif
        std::vector<const char*> GetRequiredExtensions();

    private:
        VkInstance m_VulkanInstance;

#ifdef _DEBUG
        VkDebugUtilsMessengerEXT debugMessenger;
#endif
    };
}