cmake_minimum_required(VERSION 3.28)
project(ShaderEditor)

set(CMAKE_CXX_STANDARD 17)

add_executable(ShaderEditor main.cpp
        callbacks.cpp
        callbacks.h
        Window.cpp
        Window.h
        Window.h
        GUI.cpp
        GUI.h
        GUI.h
        Shader.cpp
        Shader.h
        Shader.h
        Renderer.cpp
        Renderer.h
        Renderer.h)

find_package(OpenGL REQUIRED)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(ShaderEditor PRIVATE OpenGL32 glad::glad glfw imgui::imgui)