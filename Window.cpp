//
// Created by Scott Kirila on 2024/04/29.
//

#include "Window.h"

#include <iostream>

#include "Callbacks.h"

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
        throw std::runtime_error("Failed to create GLFW window.");
#endif
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);
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

double Window::GetTime() {
    return glfwGetTime();
}

GLFWwindow* Window::GetCurrentContext() {
    return glfwGetCurrentContext();
}

void Window::BackupCurrentContext(GLFWwindow* current_context) {
    glfwMakeContextCurrent(current_context);
}

void* Window::GetProcAddress(const char *procname) {
    return reinterpret_cast<void*>(glfwGetProcAddress(procname));
}

