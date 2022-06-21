cmake_minimum_required(VERSION 3.8)

project(aether)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/cglm)

find_package(Vulkan REQUIRED)

file(GLOB SRC
    "src/**/**/*.c"
    "src/**/*.c"
    "src/*.c"
    "src/vendor.cpp"
)

add_library(aether 
    ${SRC}
)
target_include_directories(aether PUBLIC "src" "vendor/VulkanMemoryAllocator/include" "vendor/stb")
target_link_libraries(aether PUBLIC glfw Vulkan::Vulkan cglm)