//
// Created by Scott Kirila on 2024/04/29.
//

#include "Callbacks.h"
#include "Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <exception>

Window::Window(const int viewWidth, const int viewHeight) : m_ViewWidth(viewWidth), m_ViewHeight(viewHeight) {
    glfwSetErrorCallback(Callbacks::GlfwErrorCallback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_ViewWidth, m_ViewHeight, "Shader Editor", nullptr, nullptr);

    if (!m_Window)
    {
        throw std::exception("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::exception("Failed to initialize GLAD.");
    }
}

Window::~Window() {
    std::cout << "Window\n";
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::BeginLoop(const std::function<void(void)> &fcn) const {
    while (!glfwWindowShouldClose(m_Window)) {

        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(m_Window, true);
        }

        fcn();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Window::GetWindowSize(int* x, int* y) const {
    glfwGetFramebufferSize(m_Window, x, x);
}

double Window::GetTime() {
    return glfwGetTime();
}

void Window::BackupCurrentContext() {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    glfwMakeContextCurrent(backup_current_context);
}

