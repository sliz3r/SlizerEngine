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
        int  CreateVulkanInstance();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        bool IsDeviceSuitable(const VkPhysicalDevice& device) const;
        int  RateDeviceSuitability(const VkPhysicalDevice& device, bool needToCheckForVR) const;
        bool FindQueueFamilies(const VkPhysicalDevice& device, int& queueFamilyIndex) const;

#ifdef _DEBUG
        bool CheckValidationLayerSupport();
        void SetupDebugMessenger();

        static   VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void     DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
#endif
        std::vector<const char*> GetRequiredExtensions() const;

    private:
        VkInstance m_VulkanInstance;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        //Currently we are using only one
        VkDevice m_Device;
        VkQueue m_GraphicsQueue;
        //

#ifdef _DEBUG
        VkDebugUtilsMessengerEXT debugMessenger;
#endif
    };
}