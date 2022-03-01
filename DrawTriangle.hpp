//
// Created by vince on 01/08/2021.
//

#ifndef RENDERER_DRAWTRIANGLE_HPP
#define RENDERER_DRAWTRIANGLE_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <set>
#include <string>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

class DrawTriangle {
    public:
        void run() {
            this->initWindow();
            this->initVulkan();
            this->mainLoop();
            this->cleanup();
        };
    private:
        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsFamily;

            bool isComplete() {
                return graphicsFamily.has_value();
            }
        };
        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        void initWindow() {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            this->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Renderer", nullptr, nullptr);
        };

        void initVulkan() {
            this->createInstance();
            this->pickPhysicalDevice();
            this->createLogicalDevice();
        };

        void createInstance() {
            if (enableValidationLayers && !checkValidationLayerSupport())
                throw std::runtime_error("validation layers requested, but not available!");

            VkApplicationInfo appInfo {};
            appInfo.pApplicationName = "Vulkan Renderer";
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;

            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            if (!this->areGlfwExtensionsSupported(extensions, glfwExtensions, glfwExtensionCount))
                throw std::runtime_error("Required extensions are not supported");

            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;

            if (enableValidationLayers) {
                createInfo.enabledExtensionCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            } else
                createInfo.enabledLayerCount = 0;

            if (vkCreateInstance(&createInfo, nullptr, &this->instance) == VK_SUCCESS)
                std::cout << "Vulkan instance created successfully\n" << std::endl;
            else
                throw std::runtime_error("Failed to create Vulkan instance");
        };

        void pickPhysicalDevice() {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
            if (deviceCount == 0)
                throw std::runtime_error("failed to find GPUs with Vulkan support!");
            
            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

            for (const auto& device : devices) {
                if (isDeviceSuitable(device)) {
                    physicalDevice = device;
                    break;
                }
            }

            if (physicalDevice == VK_NULL_HANDLE) {
                throw std::runtime_error("failed to find a suitable GPU!");
            }
        };

        bool isDeviceSuitable(VkPhysicalDevice device) {
            QueueFamilyIndices indices = this->findQueueFamilies(device);

            bool extensionsSupported = checkDeviceExtensionSupport(device);

            return indices.isComplete();
        };

        bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
            std::cout << "Checking device extensions" << std::endl;

            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

            for (const auto& extension : availableExtensions) {
                requiredExtensions.erase(extension.extensionName);
            }

            return requiredExtensions.empty();
        };

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
            SwapChainSupportDetails details;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, this->surface, &details.capabilities);

            return details;
        };

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
            QueueFamilyIndices indices;
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            int i = 0;
            for (const auto& queueFamily : queueFamilies) {
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    indices.graphicsFamily = i;
                }
                if (indices.isComplete())
                    break;
                i++;
            }
            return indices;
        }

        bool areGlfwExtensionsSupported(std::vector<VkExtensionProperties> extensions, const char** glfwExtensions, uint32_t glfwExtensionsCount) {
            for (uint32_t i = 0; i < glfwExtensionsCount; i++) {
                auto glfwExtension = glfwExtensions[i];
                bool found = false;
                for (auto extension : extensions) {
                    if (strcmp(extension.extensionName, glfwExtension) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return false;
            }
            return true;
        };

        void mainLoop() {
            while (!glfwWindowShouldClose(this->window)) {
                glfwPollEvents();
            }
        };

        bool checkValidationLayerSupport() {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            for (const char* layerName : validationLayers) {
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
        };

        void createLogicalDevice() {
            QueueFamilyIndices indices = this->findQueueFamilies(this->physicalDevice);

            VkDeviceQueueCreateInfo queueCreateInfo {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;
            float queuePriority = 1.0f;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            VkPhysicalDeviceFeatures deviceFeatures{};

            VkDeviceCreateInfo createInfo {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.pQueueCreateInfos = &queueCreateInfo;
            createInfo.queueCreateInfoCount = 1;
            createInfo.pEnabledFeatures = &deviceFeatures;
            createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
            createInfo.ppEnabledExtensionNames = deviceExtensions.data();
            if (enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            } else {
                createInfo.enabledLayerCount = 0;
            }

            if (vkCreateDevice(this->physicalDevice, &createInfo, nullptr, &this->device) != VK_SUCCESS)
                throw std::runtime_error("failed to create logical device!");

            vkGetDeviceQueue(this->device, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
        }

        void cleanup() {
            vkDestroyDevice(this->device, nullptr);
            vkDestroyInstance(this->instance, nullptr);
            glfwDestroyWindow(this->window);
            glfwTerminate();
        };

        GLFWwindow *window;
        VkInstance instance;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue;
        VkSurfaceKHR surface;
};

#endif //RENDERER_DRAWTRIANGLE_HPP
