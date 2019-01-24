#include "graphicsengine.h"
#include <iostream>
#include "Engine/utils.h"

namespace Engine
{
    GraphicsEngine::GraphicsEngine()
    {}

    GraphicsEngine::~GraphicsEngine()
    {
#ifdef _DEBUG
        DestroyDebugUtilsMessengerEXT(m_VulkanInstance, debugMessenger, nullptr);
#endif
        vkDestroyInstance(m_VulkanInstance, nullptr);
    }

    int GraphicsEngine::Init()
    {
        int returnValue = CreateVulkanInstance();
#ifdef _DEBUG
        SetupDebugMessenger();
#endif
        return returnValue;
    }

    int GraphicsEngine::Update(GLFWwindow* window)
    {
        return SE_CONTINUE;
    }

    int GraphicsEngine::CreateVulkanInstance()
    {
#ifdef _DEBUG
        if (!CheckValidationLayerSupport()) {
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
        createInfo.enabledLayerCount = static_cast<uint32_t>(G_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = G_ValidationLayers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif

        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance);

        if (result == VK_SUCCESS)
            return SE_CONTINUE;
        else
            return SE_ERROR;
    }

#ifdef _DEBUG
    bool GraphicsEngine::CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : G_ValidationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsEngine::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData, void * pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << ". Severity: " << messageSeverity << std::endl;
        ASSERT(messageSeverity != VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
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

    void GraphicsEngine::SetupDebugMessenger()
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = nullptr;

        if (CreateDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }
#endif

    std::vector<const char*> GraphicsEngine::GetRequiredExtensions()
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
