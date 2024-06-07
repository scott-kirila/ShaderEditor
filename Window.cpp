//
// Created by Scott Kirila on 2024/04/29.
//

#include <iostream>
#include <exception>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Callbacks.h"
#include "Window.h"

void DeleteGLFWwindow(GLFWwindow* ptr) {
    glfwDestroyWindow(ptr);
}

Window::Window(const int viewWidth, const int viewHeight) : m_ViewWidth(viewWidth), m_ViewHeight(viewHeight) {
    glfwSetErrorCallback(Callbacks::GlfwErrorCallback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_ViewWidth, m_ViewHeight, "Shader Editor", nullptr, nullptr);

    if (!m_Window)
    {
#if __APPLE__
        throw "Failed to create GLFW window.";
#elif _WIN64
        throw std::exception("Failed to create GLFW window.");
#endif

    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
#if __APPLE__
        throw "Failed to initialize GLAD.";
#elif _WIN64
        throw std::exception("Failed to initialize GLAD.");
#endif
    }
}

Window::~Window() {
    std::cout << "Window\n";
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::BeginLoop(const std::function<void(void)> &fcn) const {
    while (!glfwWindowShouldClose(m_Window)) {

        glfwPollEvents();

        fcn();

        glfwSwapBuffers(m_Window);

        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) || m_ShouldClose) {
            glfwSetWindowShouldClose(m_Window, true);
        }
    }
}

void Window::GetWindowSize(int* x, int* y) const {
    glfwGetFramebufferSize(m_Window, x, x);
}

std::function<void *(const char *)> Window::GetProcAddress() {
#if _WIN64
    return glfwGetProcAddress;
#elif __APPLE__
    return [](const char* name) -> void* {
        return reinterpret_cast<void*>(glfwGetProcAddress(name));
    };
#endif
}

double Window::GetTime() {
    return glfwGetTime();
}

GLFWwindow* Window::GetCurrentContext() {
    return glfwGetCurrentContext();
}

void Window::BackupCurrentContext(GLFWwindow* current_context) {
    glfwMakeContextCurrent(current_context);
}

