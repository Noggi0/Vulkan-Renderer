#ifndef __INCLUDES_HPP__
#define __INCLUDES_HPP__
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"

    #define GLM_FORCE_RADIANS
    #define GLM_FORCE_DEPTH_ZERO_TO_ONE
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    #define GLFW_INCLUDE_VULKAN
    #ifdef WIN32
        #define GLFW_EXPOSE_NATIVE_WIN32
    #elif __APPLE__
        #define GLFW_EXPOSE_NATIVE_COCOA
    #elif __linux__
        #define GLFW_EXPOSE_NATIVE_X11
    #endif
    #include <GLFW/glfw3.h>
    #include <GLFW/glfw3native.h>
    #include <vector>
    #include <optional>
    #include <set>
    #include <cstdint>
    #include <limits>
    #include <algorithm>
    #include <string>
    #include <iostream>
    #include <stdexcept>
    #include <array>
    #include <chrono>

#endif /* __INCLUDES_HPP__ */