cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Renderer)

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    set(_GLFW_COCOA TRUE)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("Librairies/GLFW")
add_subdirectory("Librairies/glm" EXCLUDE_FROM_ALL)
include_directories("Librairies/stb")

find_package(Vulkan REQUIRED FATAL_ERROR)

file(GLOB_RECURSE SOURCE_FILES ./main.cpp)

add_executable(Renderer ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Renderer Vulkan::Vulkan glm glfw)

if(APPLE)
    target_compile_definitions(Renderer PUBLIC VK_USE_PLATFORM_MACOS_MVK)
endif ()