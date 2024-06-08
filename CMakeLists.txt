cmake_minimum_required(VERSION 3.28)
project(ShaderEditor)

set(CMAKE_CXX_STANDARD 23)

add_executable(ShaderEditor main.cpp
        callbacks.cpp
        callbacks.h
        Window.cpp
        Window.h
        GUI.cpp
        GUI.h
        Shader.cpp
        Shader.h
        RenderingOpenGL.cpp
        RenderingOpenGL.h
        Vector.h
        RenderingBase.cpp
        RenderingBase.h
        TextCompletion.cpp
        TextCompletion.h
)

find_package(OpenGL REQUIRED)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)

if (WIN32)
    target_link_libraries(ShaderEditor PRIVATE OpenGL32 glad::glad glfw imgui::imgui)
endif (WIN32)
if (APPLE)
    # glfw handled by pkg-config; should probably check for it, regardless
    target_link_libraries(ShaderEditor PRIVATE OpenGL::GL glad::glad imgui::imgui unofficial::nativefiledialog::nfd)
endif(APPLE)
