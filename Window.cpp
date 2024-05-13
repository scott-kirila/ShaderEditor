//
// Created by Scott Kirila on 2024/04/29.
//

// SHOULD ONLY DEPEND ON GLFW (HAS MINOR DEPENDENCE ON OPENGL AT THE MOMENT)

#include "Callbacks.h"
#include "Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <exception>

Window::Window(int viewWidth, int viewHeight) : m_ViewWidth(viewWidth), m_ViewHeight(viewHeight) {
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
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::BeginLoop(void (*functionPtr)()) {
    while (!glfwWindowShouldClose(m_Window)) {

        glClearColor(0.25f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(m_Window, true);
        }

        (*functionPtr)();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

