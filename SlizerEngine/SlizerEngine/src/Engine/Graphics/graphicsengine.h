#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Engine/utils.h"
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
        NON_COPYABLE_CLASS(GraphicsEngine);
        friend class Engine;

    public:
        GraphicsEngine() = default;
        ~GraphicsEngine();

        void Init(GLFWwindow* window);
        void Update();
        void Shutdown();

    private:
        void CreateVulkanInstance();
        void CreateWindowSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();
        void Draw();
        bool IsDeviceSuitable(VkPhysicalDevice device) const;
        int  RateDeviceSuitability(VkPhysicalDevice device, bool needToCheckForVR) const;
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
        VkShaderModule CreateShaderModule(const std::vector<char>& shaderCode);

#ifdef _DEBUG
        bool CheckValidationLayerSupport() const;
        void SetupDebugMessenger();

        static   VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void     DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
#endif
        std::vector<const char*> GetRequiredExtensions() const;

    private:

        size_t m_CurrentFrame;
        GLFWwindow* m_Window;
        VkInstance m_VulkanInstance;
        VkSurfaceKHR m_WindowSurface;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
        //TODO(dcervera):Wrap VkSwapchainKHR && std::vector<VkImage> in order to have all SwapChain functionality in a single class 
        VkSwapchainKHR m_SwapChain;
        std::vector<VkImage> m_SwapChainImages;
        std::vector<VkImageView> m_SwapChainImageViews;
        VkFormat m_SwapChainImageFormat;
        VkExtent2D m_SwapChainExtent;
        std::vector<VkFramebuffer> m_SwapChainFramebuffers;
        //END TODO(dcervera)

        QueueFamilyIndices m_QueueFamilyIndices;
        VkCommandPool m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        VkPipelineLayout m_PipelineLayout;
        VkRenderPass m_RenderPass;
        VkPipeline m_GraphicsPipeline;
        const int MAX_FRAMES_IN_FLIGHT = 2;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;

#ifdef _DEBUG
        VkDebugUtilsMessengerEXT m_DebugMessenger;
#endif
    };
}