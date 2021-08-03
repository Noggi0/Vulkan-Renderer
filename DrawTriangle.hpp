//
// Created by vince on 01/08/2021.
//

#ifndef RENDERER_DRAWTRIANGLE_HPP
#define RENDERER_DRAWTRIANGLE_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class DrawTriangle {
    public:
        void run() {
            this->initWindow();
            this->initVulkan();
            this->mainLoop();
            this->cleanup();
        };
    private:
        void initVulkan() {
            this->createInstance();
        };
        void createInstance() {
            VkApplicationInfo appInfo {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Vulkan Renderer";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;
        };
        void initWindow() {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            this->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Renderer", nullptr, nullptr);
        };
        void mainLoop() {
            while (!glfwWindowShouldClose(this->window)) {
                glfwPollEvents();
            }
        };
        void cleanup() {
            glfwDestroyWindow(this->window);
            glfwTerminate();
        };
        GLFWwindow *window;
        VkInstance instance;
};

#endif //RENDERER_DRAWTRIANGLE_HPP
