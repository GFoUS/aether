cmake_minimum_required(VERSION 3.8)

project(aether)

add_definitions("-DCIMGUI_FREETYPE=1")
if (WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
    add_definitions("-D_CRT_SECURE_NO_WARNINGS=ON")
else(WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" ")
endif(WIN32)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/cglm)
add_subdirectory(vendor/nativefiledialog-extended)

find_package(Vulkan REQUIRED)

file(GLOB SRC
    "src/**/**/**/*.c"
    "src/**/**/*.c"
    "src/**/*.c"
    "src/*.c"
    "src/vendor.cpp"
)

file(GLOB IMGUI_SRC
    "vendor/cimgui/cimgui.cpp"
    "vendor/cimgui/imgui/*.cpp"
    "vendor/cimgui/imgui/backends/imgui_impl_glfw.cpp"
    "vendor/cimgui/imgui/backends/imgui_impl_vulkan.cpp"
)

file(GLOB CJSON_SRC
    "vendor/cJSON/cJSON.c"
)

add_library(aether 
    ${SRC}
    ${IMGUI_SRC}
    ${CJSON_SRC}
)
target_include_directories(aether PUBLIC 
    "src" 
    "vendor/VulkanMemoryAllocator/include" 
    "vendor/stb"
    "vendor/cimgui"
    "vendor/cimgui/imgui"
    "vendor/cimgui/generator/output"
    "vendor/cgltf"
    "vendor/cJSON"
)
target_link_libraries(aether PUBLIC glfw Vulkan::Vulkan cglm)
target_link_libraries(aether PRIVATE nfd)