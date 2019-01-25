#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <vector>

namespace Engine
{
    struct QueueFamilyIndices
    {
        uint32_t graphicsFamily = UINT32_MAX;
        uint32_t presentFamily = UINT32_MAX;

        bool IsComplete()
        {
            return graphicsFamily != UINT32_MAX && presentFamily != UINT32_MAX;
        }
    };

    class GraphicsEngine
    {
    public:
        GraphicsEngine();
        ~GraphicsEngine();

        int Init(GLFWwindow* window);
        int Update();

    private:
        int  CreateVulkanInstance();
        void CreateWindowSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        bool IsDeviceSuitable(const VkPhysicalDevice& device) const;
        int  RateDeviceSuitability(const VkPhysicalDevice& device, bool needToCheckForVR) const;
        QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device) const;
        bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;

#ifdef _DEBUG
        bool CheckValidationLayerSupport() const;
        void SetupDebugMessenger();

        static   VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void     DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
#endif
        std::vector<const char*> GetRequiredExtensions() const;

    private:

        GLFWwindow* m_Window;
        VkInstance m_VulkanInstance;
        VkSurfaceKHR m_WindowSurface;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        //Currently we are using only one
        VkDevice m_Device;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
        //TODO(dcervera):Wrap VkSwapchainKHR && std::vector<VkImage> in order to have all SwapChain functionality in a single class 
        VkSwapchainKHR m_SwapChain;
        std::vector<VkImage> m_SwapChainImages;
        VkFormat m_SwapChainImageFormat;
        VkExtent2D m_SwapChainExtent;
        //END TODO(dcervera)

#ifdef _DEBUG
        VkDebugUtilsMessengerEXT m_DebugMessenger;
#endif
    };
}