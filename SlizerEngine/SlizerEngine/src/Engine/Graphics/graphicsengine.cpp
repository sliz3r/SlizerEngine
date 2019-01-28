#include "graphicsengine.h"
#include <iostream>
#include <set>
#include "Engine/utils.h"
#include <algorithm>

//TODO(dcervera):Wrap VkSwapchainKHR && std::vector<VkImage> in order to have all SwapChain functionality in a single class 
struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    void QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& windowSurface, GLFWwindow* pWindow)
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, windowSurface, &capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowSurface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowSurface, &formatCount, formats.data());
        }

        uint32_t presentModesCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowSurface, &presentModesCount, nullptr);

        if (presentModesCount != 0)
        {
            presentModes.resize(presentModesCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowSurface, &presentModesCount, presentModes.data());
        }
    }

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat() const
    {
        //Custom values. We can edit this method in the future.
        if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        {
            return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
        }

        for (const auto& availableFormat : formats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return formats[0];
    }

    VkPresentModeKHR ChooseSwapPresentMode()
    {
        VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

        //Custom values. We can edit this method in the future.
        // 1. VK_PRESENT_MODE_MAILBOX_KHR 
        // 2. VK_PRESENT_MODE_IMMEDIATE_KHR
        // 3. VK_PRESENT_MODE_FIFO_KHR

        for (const auto& availablePresentMode : presentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
            else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
            {
                bestMode = availablePresentMode;
            }
        }

        return bestMode;
    }

    VkExtent2D ChooseSwapExtent()
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            //TODO(dcervera) Use utils.h wheh moving the whole SwapChain features to other class
            VkExtent2D actualExtent = { SCR_WIDTH, SCR_HEIGHT };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }
};
//END TODO(dcervera)

namespace Engine
{
    GraphicsEngine::GraphicsEngine()
    {}

    GraphicsEngine::~GraphicsEngine()
    {
        for (auto imageView : m_SwapChainImageViews) 
        {
            vkDestroyImageView(m_Device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
        vkDestroyDevice(m_Device, nullptr);
#ifdef _DEBUG
        DestroyDebugUtilsMessengerEXT(m_VulkanInstance, m_DebugMessenger, nullptr);
#endif
        vkDestroySurfaceKHR(m_VulkanInstance, m_WindowSurface, nullptr);
        vkDestroyInstance(m_VulkanInstance, nullptr);
    }

    int GraphicsEngine::Init(GLFWwindow* window)
    {
        if (!window)
        {
            throw std::runtime_error("failed: the window that you are sending to the graphics engine is nullptr!");
        }
        m_Window = window;

        int returnValue = CreateVulkanInstance();
#ifdef _DEBUG
        SetupDebugMessenger();
#endif
        CreateWindowSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
        CreateImageViews();
        return returnValue;
    }

    int GraphicsEngine::Update()
    {
        return SE_CONTINUE;
    }

    int GraphicsEngine::CreateVulkanInstance()
    {
#ifdef _DEBUG
        if (!CheckValidationLayerSupport()) 
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }
#endif
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Scene";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Slizer Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        auto extensions = GetRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
#ifdef _DEBUG
        createInfo.enabledLayerCount = static_cast<uint32_t>(g_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = g_ValidationLayers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif

        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance);

        if (result == VK_SUCCESS)
            return SE_CONTINUE;
        else
            return SE_ERROR;
    }

    void GraphicsEngine::CreateWindowSurface()
    {
        if (glfwCreateWindowSurface(m_VulkanInstance, m_Window, nullptr, &m_WindowSurface) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void GraphicsEngine::PickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceCount, devices.data());

        //Use RateDeviceSuitability() to get the best device depending on custom parameters.
        /*
        std::multimap<int, VkPhysicalDevice> candidates;
        for (const auto& device : devices)
        {
            int score = RateDeviceSuitability(device, false);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0) 
        {
            m_PhysicalDevice = candidates.rbegin()->second;
        }
        else 
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }*/

        for (const auto& device : devices) 
        {
            if (IsDeviceSuitable(device)) 
            {
                m_PhysicalDevice = device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE) 
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void GraphicsEngine::CreateLogicalDevice()
    {
        QueueFamilyIndices  indices = FindQueueFamilies(m_PhysicalDevice);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) 
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        //Custom this parameter in the future
        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(g_DeviceExtensions.size());
        createInfo.ppEnabledExtensionNames = g_DeviceExtensions.data();
#ifdef _DEBUG
        createInfo.enabledLayerCount = static_cast<uint32_t>(g_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = g_ValidationLayers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif
       
        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(m_Device, indices.graphicsFamily, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.presentFamily, 0, &m_PresentQueue);
    }

    void GraphicsEngine::CreateSwapChain()
    {
        SwapChainSupportDetails swapChainSupport;
        swapChainSupport.QuerySwapChainSupport(m_PhysicalDevice, m_WindowSurface, m_Window);

        VkSurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat();
        VkPresentModeKHR presentMode = swapChainSupport.ChooseSwapPresentMode();
        VkExtent2D extent = swapChainSupport.ChooseSwapExtent();

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_WindowSurface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; /*VK_IMAGE_USAGE_TRANSFER_DST_BIT ->  For postprocessing purposes*/

        QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

        if (indices.graphicsFamily != indices.presentFamily) 
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else 
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;
    }

    void GraphicsEngine::CreateImageViews()
    {
        for (int i = 0; i < m_SwapChainImages.size() ; ++i)
        {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_SwapChainImages[i];

            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_SwapChainImageFormat;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_Device, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }

    bool GraphicsEngine::IsDeviceSuitable(VkPhysicalDevice device) const
    {   
        QueueFamilyIndices indices = FindQueueFamilies(device);
        bool extensionsSupported = CheckDeviceExtensionSupport(device);
        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport;
            swapChainSupport.QuerySwapChainSupport(device, m_WindowSurface, m_Window);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.IsComplete() && swapChainAdequate;
    }

    int GraphicsEngine::RateDeviceSuitability(VkPhysicalDevice device, bool needToCheckForVR) const
    {
        //Custom this in case we need more features or properties.
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        int score = 0;

        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) 
        {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;
        
        if (!deviceFeatures.geometryShader || needToCheckForVR? !deviceFeatures.multiViewport : false)
        {
            return 0;
        }

        return score;
    }

    QueueFamilyIndices GraphicsEngine::FindQueueFamilies(VkPhysicalDevice device) const
    {
        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int queueFamilyIndex = 0;
        for (const auto& queueFamily : queueFamilies) 
        {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            {
                indices.graphicsFamily = queueFamilyIndex;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, queueFamilyIndex, m_WindowSurface, &presentSupport);

            if (queueFamily.queueCount > 0 && presentSupport) 
            {
                indices.presentFamily = queueFamilyIndex;
            }

            if (indices.IsComplete()) 
            {
                break;
            }

            ++queueFamilyIndex;
        }

        return indices;
    }

    bool GraphicsEngine::CheckDeviceExtensionSupport(VkPhysicalDevice device) const
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(g_DeviceExtensions.begin(), g_DeviceExtensions.end());
        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

#ifdef _DEBUG
    bool GraphicsEngine::CheckValidationLayerSupport() const
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        std::set<std::string> requiredLayers(g_ValidationLayers.begin(), g_ValidationLayers.end());
        for (const auto& layer : availableLayers)
        {
            requiredLayers.erase(layer.layerName);
        }

        return requiredLayers.empty();
    }

    void GraphicsEngine::SetupDebugMessenger()
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | */ VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = nullptr;

        if (CreateDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsEngine::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData, void * pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << ". Severity: " << messageSeverity << std::endl;
        ASSERT(messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
        return VK_FALSE;
    }

    VkResult GraphicsEngine::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugUtilsMessengerEXT * pDebugMessenger)
    {
        auto functor = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (functor != nullptr)
        {
            return functor(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else 
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void GraphicsEngine::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks * pAllocator)
    {
        auto functor = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (functor != nullptr)
        {
            functor(instance, debugMessenger, pAllocator);
        }
    }
#endif

    std::vector<const char*> GraphicsEngine::GetRequiredExtensions() const
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);  
#ifdef _DEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);    
#endif
        return extensions;
    }
}
